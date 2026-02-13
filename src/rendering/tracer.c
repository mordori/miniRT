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

	if (hit_bvh(ctx->scene.bvh_root, &path->ray, &path->hit, 0))
	{
		path->mat = path->hit.obj->mat;
		if (path->mat->is_emissive)
		{
			if (path->bounce == 0 || path->last_bounce_was_spec)
				path->color = vec3_add(path->color, vec3_clamp_mag(vec3_mul(path->throughput, path->mat->emission), 40.0f));
			return (false);
		}
		if (ctx->scene.lights.total > 0 && path->mat->metallic < 0.9f)
			nee(ctx, path, pixel);
		return (scatter(ctx, path, pixel));
	}
	bg_color = background_color(&ctx->scene.skydome, &path->ray, 1.0f / ctx->renderer.cam.exposure, ctx->scene.skydome_uv_offset);
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
	t_vec3		f0;
	t_vec3		fresnel;
	float		ndotv;
	float		f0_dielectric;

	random_uv(ctx, path, pixel, BN_SC_U);
	ndotv = clampf01(vec3_dot(path->hit.normal, vec3_negate(path->ray.dir)));
	f0_dielectric = reflectance(path->mat->ior);
	f0 = vec3_lerp(vec3_n(f0_dielectric), get_surface_color(path->mat, &path->hit), path->mat->metallic);
	fresnel = vec3_schlick(f0, ndotv);
	specular_probability(path, pixel, fresnel);
	if (path->last_bounce_was_spec)
	{
		if (!bounce_specular(path, f0, ndotv))
			return (false);
	}
	else
		bounce_diffuse(path);
	path->ray = new_ray(vec3_bias(path->hit.point, path->hit.normal), path->dir_bounce);
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
