#include "editing.h"

static inline void	x_constraint(t_context *ctx);
static inline void	y_constraint(t_context *ctx);
static inline void	z_constraint(t_context *ctx);
static inline t_vec3	get_contraint_axis(t_context *ctx, t_vec3 global_axis, uint32_t col);

void	set_axis_constraints(t_context *ctx, mlx_key_data_t keydata)
{
	ctx->editor.has_constraints = false;
	if (keydata.key != MLX_KEY_X && keydata.key != MLX_KEY_Y && keydata.key != MLX_KEY_Z)
		return ;
	ctx->editor.axis_secondary = g_zero;
	if (keydata.key == MLX_KEY_X)
		x_constraint(ctx);
	else if (keydata.key == MLX_KEY_Y)
		y_constraint(ctx);
	else if (keydata.key == MLX_KEY_Z)
		z_constraint(ctx);
	ctx->editor.has_constraints = true;
}

static inline void	planar_basis(t_context *ctx, t_vec3 normal)
{
	t_vec3		plane_x;
	t_vec3		plane_y;
	float		dot_x;
	float		dot_y;

	dot_x = vec3_dot(ctx->scene.cam.right, normal);
	plane_x = vec3_sub(ctx->scene.cam.right, vec3_scale(normal, dot_x));
	if (vec3_dot(plane_x, plane_x) < 0.001f)
	{
		dot_x = vec3_dot(ctx->scene.cam.forward, normal);
		plane_x = vec3_sub(ctx->scene.cam.forward, vec3_scale(normal, dot_x));
	}
	dot_y = vec3_dot(ctx->scene.cam.up, normal);
	plane_y = vec3_sub(ctx->scene.cam.up, vec3_scale(normal, dot_y));
	if (vec3_dot(plane_y, plane_y) < 0.001f)
	{
		dot_y = vec3_dot(ctx->scene.cam.forward, normal);
		plane_y = vec3_sub(ctx->scene.cam.forward, vec3_scale(normal, dot_y));
	}
	if (vec3_dot(plane_x, plane_x) > 0.0001f)
		plane_x = vec3_normalize(plane_x);
	else
		plane_x = g_zero;
	if (vec3_dot(plane_y, plane_y) > 0.0001f)
		plane_y = vec3_normalize(plane_y);
	else
		plane_y = g_zero;

	ctx->editor.axis_primary = plane_x;
	ctx->editor.axis_secondary = plane_y;
}

static inline t_vec3	get_contraint_axis(t_context *ctx, t_vec3 global_axis, uint32_t col)
{
	t_mat4		*m;
	t_vec3		axis;

	if (ctx->editor.mode == EDIT_SCALE)
	{
		m = &ctx->editor.selected_obj->transform.object_to_world;
		axis = vec3(m->m[0][col], m->m[1][col], m->m[2][col]);
		axis = vec3_normalize(axis);
		return (axis);
	}
	return (global_axis);
}

static inline void	x_constraint(t_context *ctx)
{
	ctx->editor.axis_primary = get_contraint_axis(ctx, g_right, 0);
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_SHIFT))
		planar_basis(ctx, g_right);
}

static inline void	y_constraint(t_context *ctx)
{
	ctx->editor.axis_primary = get_contraint_axis(ctx, g_up, 1);
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_SHIFT))
		planar_basis(ctx, g_up);
}

static inline void	z_constraint(t_context *ctx)
{
	ctx->editor.axis_primary = get_contraint_axis(ctx, g_forward, 2);
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_SHIFT))
		planar_basis(ctx, g_forward);
}
