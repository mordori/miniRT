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

	if ((int)hit_object(ctx->scene.selected_obj, &path->ray, &path->hit) | \
(int)hit_object(ctx->renderer.cam.directional_light.obj, &path->ray, &path->hit) | \
(int)hit_bvh(ctx->scene.bvh_root, &path->ray, &path->hit, 0))
	{
		path->mat = path->hit.obj->mat;
		set_material_data(path);
		if (path->mat->is_emissive)
		{
			light_emission = vec3_mul(path->throughput, path->mat->emission);
			if (path->bounce > 0)
				light_emission = vec3_scale(light_emission, (1.0f - path->mat->roughness));
			light_emission = vec3_clamp_mag(light_emission, 2.0f);
			path->color = vec3_add(path->color, vec3_mul(path->throughput, light_emission));
			return (false);
		}
		if (path->bounce > 0)
			return (false);
		if (ctx->scene.has_directional_light)
			add_lighting_editing(ctx, path, &ctx->renderer.cam.directional_light);
		if (ctx->scene.lights.total > 0)
			nee_editing(ctx, path);
		ambient_lighting(path, &ctx->scene.ambient_light);
		return (scatter_editing(path));
	}
	if (path->bounce > 0)
		return (false);
	bg_color = background_color(&ctx->scene.skydome, &path->ray, ctx->renderer.cam.skydome_uv_offset);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, bg_color));
	return (false);
}

static inline void	nee_editing(const t_context *ctx, t_path *path)
{
	const t_light	*light;
	size_t			i;

	i = 0;
	while (i < ctx->scene.lights.total)
	{
		light = ((t_light **)ctx->scene.lights.items)[i++];
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
