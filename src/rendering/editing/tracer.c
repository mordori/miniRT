#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "materials.h"

static inline void	nee_editing(const t_context *ctx, t_path *path, float rough);
static inline bool	scatter_editing(t_path *path);

bool	trace_ray_editing(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	t_vec3		bg_color;
	t_vec3		light_emission;
	float		rough;
	// float		attenuation;
	float		*mask;

	if ((int)hit_object(ctx->selected_obj, &path->ray, &path->hit) | \
(int)hit_object(ctx->renderer.cam.directional_light.obj, &path->ray, &path->hit) | \
(int)hit_bvh(ctx->scene.geo.bvh_root_idx, &path->ray, &path->hit, 0, ctx->scene.geo.bvh_nodes) | \
(int)hit_planes(ctx, &path->ray, &path->hit))
	{
		if (path->bounce == 0)
		{
			mask = &ctx->selection_mask[pixel->y * ctx->renderer.width + pixel->x];
			if (path->hit.obj == ctx->selected_obj)
				*mask = path->hit.t;
			else
				*mask = -path->hit.t;
		}
		rough = 0.0f;
		if (path->bounce > 0)
			rough = path->mat->roughness;
		path->mat = path->hit.obj->mat;
		set_material_data(path);
		if (path->mat->is_emissive)
		{
			light_emission = vec3_mul(path->throughput, path->mat->emission);
			// if (path->bounce > 0)
			{
				// attenuation = powf(fmaxf(0.0f, 1.0f - rough), 10.0f) * 0.1f;
				// light_emission = vec3_scale(light_emission, attenuation);
			}
			light_emission = vec3_clamp_mag(light_emission, 2.0f);
			if (path->bounce > 0)
				light_emission = vec3_scale(light_emission, fminf(1.0f - rough, 0.1f));
			path->color = vec3_add(path->color, light_emission);
			return (false);
		}
		if (ctx->scene.env.has_dir_light)
			add_lighting_editing(ctx, path, &ctx->renderer.cam.directional_light, rough);
		if (ctx->scene.env.lights.total > 0)
			nee_editing(ctx, path, rough);
		ambient_lighting(path, &ctx->scene.env.amb_light);
		return (scatter_editing(path));
	}
	if (path->bounce > 0)
		return (false);
	ctx->selection_mask[pixel->y * ctx->renderer.width + pixel->x] = -M_INF;
	bg_color = background_color(&ctx->scene, &path->ray, ctx->renderer.cam.skydome_uv_offset);
	// bg_color = vec3_scale(bg_color, ctx->scene.env.amb_light.intensity);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, bg_color));
	return (false);
}

static inline void	nee_editing(const t_context *ctx, t_path *path, float rough)
{
	const t_light	*light;
	size_t			i;

	i = 0;
	while (i < ctx->scene.env.lights.total)
	{
		light = ((t_light **)ctx->scene.env.lights.items)[i++];
		add_lighting_editing(ctx, path, light, rough);
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
