#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "materials.h"

static inline void	nee_editing(const t_context *ctx, t_path *path);
static inline bool	scatter_editing(t_path *path);

bool	trace_ray_editing(const t_context *ctx, t_path *path)
{
	t_vec3		bg_color;
	t_vec3		light_emission;
	float		rough;
	float		attenuation;

	if ((int)hit_object(ctx->selected_obj, &path->ray, &path->hit) | \
(int)hit_object(ctx->renderer.cam.directional_light.obj, &path->ray, &path->hit) | \
(int)hit_bvh(ctx->scene.geo.bvh_root_idx, &path->ray, &path->hit, 0, ctx->scene.geo.bvh_nodes) | \
(int)hit_planes(ctx, &path->ray, &path->hit))
	{
		if (path->bounce > 0)
			rough = path->mat->roughness;
		path->mat = path->hit.obj->mat;
		set_material_data(path);
		if (path->mat->is_emissive)
		{
			light_emission = vec3_mul(path->throughput, path->mat->emission);
			if (path->bounce > 0)
			{
				attenuation = powf(fmaxf(0.0f, 1.0f - rough), 10.0f);
				light_emission = vec3_scale(light_emission, attenuation);
			}
			light_emission = vec3_clamp_mag(light_emission, 2.0f);
			path->color = vec3_add(path->color, light_emission);
			return (false);
		}
		if (path->bounce > 0)
			return (false);
		if (ctx->scene.env.has_directional_light)
			add_lighting_editing(ctx, path, &ctx->renderer.cam.directional_light);
		if (ctx->scene.env.lights.total > 0)
			nee_editing(ctx, path);
		ambient_lighting(path, &ctx->scene.env.ambient_light);
		return (scatter_editing(path));
	}
	if (path->bounce > 0)
		return (false);
	bg_color = background_color(&ctx->scene.env.skydome, &path->ray, ctx->renderer.cam.skydome_uv_offset);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, bg_color));
	return (false);
}

static inline void	nee_editing(const t_context *ctx, t_path *path)
{
	const t_light	*light;
	size_t			i;

	i = 0;
	while (i < ctx->scene.env.lights.total)
	{
		light = ((t_light **)ctx->scene.env.lights.items)[i++];
		add_lighting_editing(ctx, path, light);
	}
}

static inline bool	scatter_editing(t_path *path)
{
	path->l = vec3_reflect(path->ray.dir, path->n);
	path->ndotl = clampf01(vec3_dot(path->n, path->l));
	if (path->ndotl <= 1e-12f)
		return (false);
	path->throughput = vec3_scale(path->throughput, M_1_PI);
	path->ray = new_ray(vec3_bias(path->hit.point, path->n), path->l);
	++path->bounce;
	return (true);
}
