#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "libft_random.h"

static inline t_vec3	background_color(const t_texture *tex, const t_ray *ray, float lux);
static inline t_vec3	background_gradient(float t);
static inline bool	trace_ray(const t_context *ctx, t_path *path, t_pixel *pixel);
static inline bool	trace_ray_preview(const t_context *ctx, t_path *path);
static inline bool	scatter(t_path *path, uint32_t *seed);

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
		if (r->mode == RENDER_REFINE && !trace_ray(ctx, &path, pixel))
			break;
		else if (r->mode == RENDER_PREVIEW)
		{
			trace_ray_preview(ctx, &path);
			break ;
		}
	}
	return (path.color);
}

static inline bool	trace_ray_preview(const t_context *ctx, t_path *path)
{
	if (hit_object(ctx->scene.selected_obj, &path->ray, &path->hit) | hit_bvh(ctx->scene.bvh_root, &path->ray, &path->hit, 0))
	{
		path->mat = ((t_material **)ctx->scene.materials.items)[path->hit.obj->material_id];
		if (path->mat->is_emissive)
		{
			path->color = vec3_add(path->color, vec3_mul(path->throughput, path->mat->emission));
			return (false);
		}
		path->color = vec3_add(path->color, vec3_mul(path->throughput, compute_ambient(&ctx->scene, path->mat)));
		path->color = vec3_add(path->color, vec3_mul(path->throughput, compute_directional(&ctx->scene, &path->hit, path->mat)));
		// Add all point lights
		return (true);
	}
	path->color = vec3_add(path->color, vec3_mul(path->throughput, background_color(&ctx->scene.skydome, &path->ray, 1.0f)));
	return (true);
}

static inline bool	trace_ray(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	t_vec3		lighting;

	if (hit_object(ctx->scene.selected_obj, &path->ray, &path->hit) | hit_bvh(ctx->scene.bvh_root, &path->ray, &path->hit, 0))
	{
		path->mat = ((t_material **)ctx->scene.materials.items)[path->hit.obj->material_id];
		if (path->mat->is_emissive)
		{
			if (path->bounce == 0)
				path->color = vec3_add(path->color, vec3_mul(path->throughput, path->mat->emission));
			return (false);
		}
		lighting = compute_lighting(ctx, path, (t_light *)&ctx->scene.directional_light, pixel);
		if (path->bounce > 0)
			lighting = vec3_min(lighting, 10.0f);
		path->color = vec3_add(path->color, vec3_mul(path->throughput, lighting));
		if (ctx->scene.lights.total > 0)
		{
			lighting = compute_lighting(ctx, path, ((t_light **)ctx->scene.lights.items)[pcg(pixel->seed) % ctx->scene.lights.total], pixel);
			if (path->bounce > 0)
				lighting = vec3_min(lighting, 10.0f);
			path->color = vec3_add(path->color, vec3_mul(path->throughput, lighting));
		}
		return (scatter(path, pixel->seed));
	}
	path->color = vec3_add(path->color, vec3_mul(path->throughput, background_color(&ctx->scene.skydome, &path->ray, 20.0f)));
	return (false);
}

static inline bool	scatter(t_path *path, uint32_t *seed)
{
	t_vec3		origin;
	t_vec3		dir;
	float		len_sq;
	float		p;

	dir = vec3_add(path->hit.normal, vec3_unit_random(seed));
	len_sq = vec3_dot(dir, dir);
	if (len_sq < LEN_SQ_EPSILON)
	{
		dir = path->hit.normal;
		len_sq = 1.0f;
	}
	origin = vec3_add(path->hit.point, vec3_scale(path->hit.normal, G_EPSILON));
	path->ray = new_ray(origin, vec3_div(dir, sqrtf(len_sq)));
	path->throughput = vec3_mul(path->throughput, path->mat->albedo);
	p = fmaxf(path->throughput.r, fmaxf(path->throughput.g, path->throughput.b));
	p = ft_clamp(p, 0.05f, 0.95f);
	if (randomf01(seed) > p)
		return (false);
	path->throughput = vec3_scale(path->throughput, 1.0f / p);
	++path->bounce;
	return (true);
}

t_ray	new_ray(t_vec3 origin, t_vec3 dir)
{
	t_ray		ray;
	t_f_int		sign;

	ray.origin = origin;
	ray.dir = dir;
	ray.inv_dir.x = 1.0f / (ray.dir.x + copysignf(1e-20f, ray.dir.x));
	ray.inv_dir.y = 1.0f / (ray.dir.y + copysignf(1e-20f, ray.dir.y));
	ray.inv_dir.z = 1.0f / (ray.dir.z + copysignf(1e-20f, ray.dir.z));
	sign.f = ray.inv_dir.x;
	ray.sign[0] = sign.i >> 31;
	sign.f = ray.inv_dir.y;
	ray.sign[1] = sign.i >> 31;
	sign.f = ray.inv_dir.z;
	ray.sign[2] = sign.i >> 31;
	return (ray);
}

static inline t_vec3	background_color(const t_texture *tex, const t_ray *ray, float lux)
{
	t_float2	uv;
	t_int2		xy;
	uint32_t	i;
	t_vec3		result;
	const float	*pixels;

	if (!tex)
		return (background_gradient((ray->dir.y + 1.0f) * 0.5f));
	uv.u = ft_clamp01((atan2f(ray->dir.z, ray->dir.x) + M_PI) * M_1_2PI);
	uv.v = ft_clamp01(acosf(ray->dir.y) * M_1_PI);
	xy.x = (uint32_t)(uv.u * (tex->width - 1));
	xy.y = (uint32_t)(uv.v * (tex->height - 1));
	i = (xy.y * tex->width + xy.x) * 4;
	pixels = (const float *)__builtin_assume_aligned(tex->pixels, 64);
	memcpy(&result, &pixels[i], sizeof(t_vec3));
	return (vec3_scale(result, lux));
}

static inline t_vec3	background_gradient(float t)
{
	t_vec3		result;
	uint32_t	c;

	c = lerp_color(RED, BLUE, t);
	result.r = (float)(((c >> 24) & 0xFF) * INV_255F);
	result.g = (float)(((c >> 16) & 0xFF) * INV_255F);
	result.b = (float)(((c >> 8) & 0xFF) * INV_255F);
	return (result);
}
