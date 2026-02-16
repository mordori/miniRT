#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "materials.h"

static inline bool	trace_ray(const t_context *ctx, t_path *path, t_pixel *pixel);
static inline void	nee(const t_context *ctx, t_path *path, t_pixel *pixel);
static inline bool	scatter(const t_context *ctx, t_path *path, t_pixel *pixel);
static inline bool	russian_roulette(t_path *path, t_pixel *pixel);

t_vec3	trace_path(const t_context *ctx, t_pixel *pixel)
{
	const t_viewport	*vp;
	const t_renderer	*r;
	t_path				path;
	t_vec3				pixel_loc;

	r = &ctx->renderer;
	vp = &r->cam.viewport;
	pixel_loc = vec3_add(vec3_scale(vp->d_u, pixel->u), vec3_scale(vp->d_v, pixel->v));
	pixel_loc = vec3_add(vp->pixel_00_loc, pixel_loc);
	path = (t_path){0};
	path.ray = new_ray(r->cam.transform.pos, vec3_normalize(vec3_sub(pixel_loc, r->cam.transform.pos)));
	path.throughput = (t_vec3){{1.0f, 1.0f, 1.0f}};
	path.mode = r->mode;
	while (path.bounce < r->ray_bounces)
	{
		path.hit = (t_hit){0};
		path.hit.t = M_INF;
		path.hit.is_primary = (path.bounce == 0);
		if (!trace_ray(ctx, &path, pixel))
			break ;
	}
	return (path.color);
}

static inline bool	trace_ray(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	t_vec3		bg_color;
	float		weight;
	float		pdf;
	t_vec3		indirect_light;

	if ((int)hit_object(ctx->renderer.cam.directional_light.obj, &path->ray, &path->hit) | (int)hit_bvh(ctx->scene.bvh_root, &path->ray, &path->hit, 0))
	{
		path->mat = path->hit.obj->mat;
		set_material_data(path);
		if (path->mat->is_emissive)
		{
			weight = 1.0f;
			if (path->bounce > 0)
			{
				pdf = light_pdf(vec3_sub(path->ray.origin, path->n), path->hit.obj->shape.sphere.radius_sq);
				if (ctx->scene.lights.total > 0)
					pdf /= (float)ctx->scene.lights.total;
				weight = power_heuristic(path->pdf, pdf);
			}
			indirect_light = vec3_mul(path->throughput, vec3_scale(path->mat->emission, weight));
			if (path->bounce > 0)
				indirect_light = vec3_clamp_mag(indirect_light, MAX_BRIGHTNESS);
			path->color = vec3_add(path->color, indirect_light);
			return (false);
		}
		if (ctx->scene.has_directional_light)
			add_lighting(ctx, path, &ctx->renderer.cam.directional_light, pixel);
		if (ctx->scene.lights.total > 0)
			nee(ctx, path, pixel);
		return (scatter(ctx, path, pixel));
	}
	bg_color = background_color(&ctx->scene.skydome, &path->ray, 1.0f / ctx->renderer.cam.exposure, ctx->renderer.cam.skydome_uv_offset);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, bg_color));
	return (false);
}

static inline void	nee(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	const t_light	*light;
	size_t			i;

	if (ctx->renderer.mode == RENDER_PREVIEW)
	{
		i = (uint32_t)(blue_noise(&ctx->tex_bn, pixel, BN_LI) * ctx->scene.lights.total);
		if (i >= ctx->scene.lights.total)
			i = ctx->scene.lights.total - 1;
		light = ((t_light **)ctx->scene.lights.items)[i];
		add_lighting(ctx, path, light, pixel);
		path->color = vec3_scale(path->color, (float)ctx->scene.lights.total);
	}
	else
	{
		i = 0;
		while (i < ctx->scene.lights.total)
		{
			light = ((t_light **)ctx->scene.lights.items)[i];
			add_lighting(ctx, path, light, pixel);
			++i;
		}
	}
}

static inline bool	scatter(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	t_vec3		f;
	float		p;

	random_uv(ctx, path, pixel, BN_SC_U);
	f = vec3_schlick(path->f0, path->ndotv);
	p = fmaxf(fmaxf(f.r, f.g), f.b);
	path->p_spec = clampf(p, 0.1f, 0.9f);
	if (path->mat->metallic >= 0.9f)
		path->p_spec = 1.0f;
	path->sample_spec = randomf01(pixel->seed) < path->p_spec;
	if (!sample_bsdf(path))
		return (false);
	path->ray = new_ray(vec3_bias(path->hit.point, path->n), path->l);
	return (russian_roulette(path, pixel));
}

static inline bool	russian_roulette(t_path *path, t_pixel *pixel)
{
	float		p;

	if (path->bounce >= DEPTH_ENABLE_RR)
	{
		p = fmaxf(path->throughput.r, fmaxf(path->throughput.g, path->throughput.b));
		p = clampf(p, 0.05f, 0.95f);
		if (randomf01(pixel->seed) > p)
			return (false);
		path->throughput = vec3_scale(path->throughput, 1.0f / p);
	}
	++path->bounce;
	return (true);
}
