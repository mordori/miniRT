#include "rendering.h"
#include "materials.h"
#include "utils.h"

static inline t_vec3	background_gradient(const t_scene *scene,const float t);

t_vec3	background_color(const t_scene *scene, const t_ray *ray, t_vec2 uv_offset)
{
	t_vec2		uv;

	if (!scene->env.skydome.pixels)
		return (background_gradient(scene, (ray->dir.y + 1.0f) * 0.5f));
	uv = spherical_uv(ray->dir);
	uv = vec2_add(uv, uv_offset);
	return (sample_texture(&scene->env.skydome, uv));
}

static inline t_vec3	background_gradient(const t_scene *scene,const float t)
{
	t_vec3		res;

	res = vec3_lerp(scene->env.amb_color_2, scene->env.amb_light.color, t);
	return (res);
}

bool	rotate_skydome(t_context *ctx)
{
	static t_vec3 initial_pos = (t_vec3){{704000.0f, 484000.0f, 520000.0f, 0.0f}};
	t_light		*light;
	float		delta;
	t_vec2		theta;

	if (\
(mlx_is_key_down(ctx->mlx, MLX_KEY_PERIOD) && !mlx_is_key_down(ctx->mlx, MLX_KEY_COMMA)) || \
(mlx_is_key_down(ctx->mlx, MLX_KEY_COMMA) && !mlx_is_key_down(ctx->mlx, MLX_KEY_PERIOD)))
	{
		delta = 0.085f * ctx->mlx->delta_time;
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_COMMA))
			delta = -delta;
		ctx->scene.cam.skydome_uv_offset.u += delta;
		if (ctx->scene.env.has_dir_light)
		{
			light = &ctx->scene.cam.directional_light;
			sincosf(ctx->scene.cam.skydome_uv_offset.u * M_TAU, &theta.sin, &theta.cos);
			light->pos.x = initial_pos.x * theta.cos + initial_pos.z * theta.sin;
			light->pos.y = initial_pos.y;
			light->pos.z = -initial_pos.x * theta.sin + initial_pos.z * theta.cos;
			light->obj->transform.pos = light->pos;
			light->obj->shape.sphere.center = light->pos;
		}
		return (true);
	}
	return (false);
}
