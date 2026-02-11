#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "materials.h"

static inline bool	trace_ray(const t_context *ctx, t_path *path, t_pixel *pixel);
static inline bool	scatter(const t_context *ctx, t_path *path, t_pixel *pixel);

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
			break;
	}
	return (path.color);
}

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
			if (path->bounce == 0 || path->last_bounce_was_spec)
				path->color = vec3_add(path->color, vec3_clamp_mag(vec3_mul(path->throughput, path->mat->emission), 40.0f));
			return (false);
		}
		if (ctx->scene.lights.total > 0 && path->mat->metallic < 0.9f)
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
	path->color = vec3_add(path->color, vec3_mul(path->throughput, background_color(&ctx->scene.skydome, &path->ray, 1.0f / ctx->renderer.cam.exposure)));
	return (false);
}

// https://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
static float	g_schlick_ggx(float ndotv, float roughness)
{
	float		alpha;
	float		k;

	alpha = roughness * roughness;
	k = alpha / 2.0f;
	return (ndotv / (ndotv * (1.0f - k) + k + G_EPSILON));
}

// WIP
static inline bool	scatter(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	t_vec3		origin;
	t_vec3		dir;
	float		p;
	t_vec2		uv;

	t_vec3		v;
	t_vec3		h;
	t_vec3		f0;
	t_vec3		fresnel;
	float		p_spec;
	float		ndotv;
	float		ndotl;
	float		vdoth;
	float		ndoth;

	float		g;
	float		weight;

	float		f0_dielectric;
	t_vec3		surface_color;

	if (path->bounce == 0)
		uv = vec2(blue_noise(&ctx->tex_bn, pixel, BN_SC_U), blue_noise(&ctx->tex_bn, pixel, BN_SC_V));
	else
		uv = vec2(randomf01(pixel->seed), randomf01(pixel->seed));

	v = vec3_negate(path->ray.dir);
	ndotv = clampf01(vec3_dot(path->hit.normal, v));
	f0_dielectric = (path->mat->ior - 1.0f) / (path->mat->ior + 1.0f);
	f0_dielectric *= f0_dielectric;
	surface_color = get_surface_color(path->mat, &path->hit);
	f0 = vec3_lerp(vec3_n(f0_dielectric), surface_color, path->mat->metallic);
	fresnel = vec3_schlick(f0, ndotv);
	p_spec = clampf01((fresnel.r + fresnel.g + fresnel.b) / 3.0f);
	if (path->mat->metallic > 0.9f)
	{
		p_spec = 1.0f;
		path->last_bounce_was_spec = true;
	}
	else if (path->mat->roughness > 0.9f)
	{
		p_spec = 0.0f;
		path->last_bounce_was_spec = false;
	}
	else
	{
		p_spec = clampf(p_spec, 0.01f, 0.99f);
		path->last_bounce_was_spec = randomf01(pixel->seed) < p_spec;
	}
	if (path->last_bounce_was_spec)
	{
		h = sample_ggx(path->hit.normal, path->mat->roughness, uv);
		dir = vec3_reflect(path->ray.dir, h);
		ndotl = vec3_dot(path->hit.normal, dir);
		if (ndotl <= 0.0f)
			return (false);
		ndoth = clampf01(vec3_dot(path->hit.normal, h));
		vdoth = clampf01(vec3_dot(v, h));
		g = g_schlick_ggx(ndotv, path->mat->roughness) * g_schlick_ggx(ndotl, path->mat->roughness);
		weight = (g * vdoth) / (ndotv * ndoth + G_EPSILON);
		path->throughput = vec3_mul(path->throughput, fresnel);
		path->throughput = vec3_scale(path->throughput, weight);
		path->throughput = vec3_scale(path->throughput, 1.0f / p_spec);
	}
	else
	{
		dir = sample_cos_hemisphere(path->hit.normal, uv);
		path->throughput = vec3_mul(path->throughput, surface_color);
		if (path->mat->metallic > 0.0f)
			path->throughput = vec3_scale(path->throughput, 1.0f - path->mat->metallic);
		path->throughput = vec3_scale(path->throughput, 1.0f / (1.0f - p_spec));
	}
	origin = vec3_add(path->hit.point, vec3_scale(path->hit.normal, B_EPSILON));
	path->ray = new_ray(origin, dir);
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
