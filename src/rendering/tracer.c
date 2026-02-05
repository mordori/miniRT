#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"

static inline bool	trace_ray(const t_context *ctx, t_path *path, t_pixel *pixel);
// static inline void	trace_ray_edit(const t_context *ctx, t_path *path);
static inline bool	scatter(const t_context *ctx, t_path *path, t_pixel *pixel);
static inline t_vec3	sample_cos_hemisphere(const t_vec3 normal, const float u, const float v);

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
		// if (r->mode == RENDER_EDIT)
		// {
		// 	trace_ray_edit(ctx, &path);
		// 	break ;
		// }
		if (!trace_ray(ctx, &path, pixel))
			break;
	}
	return (path.color);
}

// static inline void	trace_ray_edit(const t_context *ctx, t_path *path)
// {
// 	if (hit_object(ctx->scene.selected_obj, &path->ray, &path->hit) | hit_bvh(ctx->scene.bvh_root, &path->ray, &path->hit, 0))
// 	{
// 		path->mat = ((t_material **)ctx->scene.materials.items)[path->hit.obj->material_id];
// 		if (path->mat->is_emissive)
// 		{
// 			path->color = vec3_add(path->color, vec3_mul(path->throughput, path->mat->emission));
// 			return ;
// 		}
// 		path->color = vec3_add(path->color, vec3_mul(path->throughput, compute_ambient(&ctx->scene, path->mat)));
// 		path->color = vec3_add(path->color, vec3_mul(path->throughput, compute_directional(&ctx->scene, &path->hit, path->mat)));
// 		// Add all point lights
// 		return ;
// 	}
// 	path->color = vec3_add(path->color, vec3_mul(path->throughput, background_color(&ctx->scene.skydome, &path->ray, 1.0f)));
// 	return ;
// }

static inline void	add_lighting(const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel)
{
	const float		max_brightness = 40.0f;
	t_vec3			lighting;

	lighting = compute_lighting(ctx, path, light, pixel);
	if (path->bounce > 0)
		lighting = vec3_clamp_mag(lighting, max_brightness);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, lighting));
}

static inline bool	trace_ray(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	uint32_t		light_idx;
	size_t			i;

	i = 0;
	if (hit_bvh(ctx->scene.bvh_root, &path->ray, &path->hit, 0))
	{
		path->mat = ((t_material **)ctx->scene.materials.items)[path->hit.obj->material_id];
		if (path->mat->is_emissive)
		{
			if (path->bounce == 0)
				path->color = vec3_add(path->color, vec3_mul(path->throughput, path->mat->emission));
			return (false);
		}
		if (ctx->scene.directional_light.obj->flags & OBJ_VISIBLE)
			add_lighting(ctx, path, (t_light *)&ctx->scene.directional_light, pixel);
		if (ctx->scene.lights.total > 0)
		{
			if (ctx->renderer.mode == RENDER_PREVIEW)
			{
				light_idx = (uint32_t)(blue_noise(&ctx->tex_bn, pixel, BN_LI) * ctx->scene.lights.total);
				if (light_idx >= ctx->scene.lights.total)
					light_idx = ctx->scene.lights.total - 1;
				add_lighting(ctx, path, ((t_light **)ctx->scene.lights.items)[light_idx], pixel);
				path->color = vec3_scale(path->color, (float)ctx->scene.lights.total);
			}
			else
				while (i < ctx->scene.lights.total)
					add_lighting(ctx, path, ((t_light **)ctx->scene.lights.items)[i++], pixel);
		}
		return (scatter(ctx, path, pixel));
	}
	path->color = vec3_add(path->color, vec3_mul(path->throughput, background_color(&ctx->scene.skydome, &path->ray, 20.0f)));
	return (false);
}

static inline t_vec3	sample_cos_hemisphere(const t_vec3 normal, const float u, const float v)
{
	float		cos_theta;
	float		sin_theta;
	t_vec3		dir_local;
	t_vec3		dir_world;
	t_vec3		t;
	t_vec3		b;
	t_vec2		phi;

	cos_theta = sqrtf(v);
	sin_theta = sqrtf(fmaxf(0.0f, 1.0f - v));
	sincosf(M_TAU * u, &phi.sin, &phi.cos);
	dir_local = vec3(sin_theta * phi.cos, sin_theta * phi.sin, cos_theta);
	orthonormal_basis(normal, &t, &b);
	dir_world = vec3_add(vec3_scale(t, dir_local.x), vec3_scale(b, dir_local.y));
	dir_world = vec3_add(dir_world, vec3_scale(normal, dir_local.z));
	return (vec3_normalize(dir_world));
}

static inline bool	scatter(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	t_vec3		origin;
	t_vec3		dir;
	float		p;
	t_vec2		uv;

	if (path->bounce == 0)
		uv = vec2(blue_noise(&ctx->tex_bn, pixel, BN_SC_U), blue_noise(&ctx->tex_bn, pixel, BN_SC_V));
	else
		uv = vec2(randomf01(pixel->seed), randomf01(pixel->seed));
	dir = sample_cos_hemisphere(path->hit.normal, uv.u, uv.v);
	origin = vec3_add(path->hit.point, vec3_scale(path->hit.normal, B_EPSILON));
	path->ray = new_ray(origin, dir);
	path->throughput = vec3_mul(path->throughput, path->mat->albedo);
	if (path->bounce >= DEPTH_ENABLE_RR)
	{
		p = fmaxf(path->throughput.r, fmaxf(path->throughput.g, path->throughput.b));
		p = ft_clamp(p, 0.05f, 0.95f);
		if (randomf01(pixel->seed) > p)
			return (false);
		path->throughput = vec3_scale(path->throughput, 1.0f / p);
	}
	++path->bounce;
	return (true);
}
