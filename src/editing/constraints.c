#include "editing.h"

static inline void	x_constraint(t_context *ctx);
static inline void	y_constraint(t_context *ctx);
static inline void	z_constraint(t_context *ctx);

void	set_axis_constraints(t_context *ctx, mlx_key_data_t keydata)
{
	ctx->editor.axis_secondary = g_zero;
	if (keydata.key == MLX_KEY_X)
		x_constraint(ctx);
	else if (keydata.key == MLX_KEY_Y)
		y_constraint(ctx);
	else if (keydata.key == MLX_KEY_Z)
		z_constraint(ctx);
}

static inline void	x_constraint(t_context *ctx)
{
	t_vec3		right;
	t_vec3		up;
	float		right_align;
	float		up_align;

	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_SHIFT))
	{
		right = ctx->scene.cam.right;
		right.x = 0.0f;
		ctx->editor.axis_primary = vec3_normalize(right);
		up = ctx->scene.cam.up;
		up.x = 0.0f;
		ctx->editor.axis_secondary = vec3_normalize(up);
	}
	else
	{
		right_align = vec3_dot(ctx->scene.cam.right, g_right);
		up_align = vec3_dot(ctx->scene.cam.up, g_right);
		ctx->editor.axis_primary = vec3_scale(g_right, right_align);
		ctx->editor.axis_secondary = vec3_scale(g_right, up_align);
	}
}

static inline void	y_constraint(t_context *ctx)
{
	t_vec3		right;
	t_vec3		up;
	float		right_align;
	float		up_align;

	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_SHIFT))
	{
		right = ctx->scene.cam.right;
		right.y = 0.0f;
		ctx->editor.axis_primary = vec3_normalize(right);
		up = ctx->scene.cam.up;
		up.y = 0.0f;
		ctx->editor.axis_secondary = vec3_normalize(up);
	}
	else
	{
		right_align = vec3_dot(ctx->scene.cam.right, g_up);
		up_align = vec3_dot(ctx->scene.cam.up, g_up);
		ctx->editor.axis_primary = vec3_scale(g_up, right_align);
		ctx->editor.axis_secondary = vec3_scale(g_up, up_align);
	}
}

static inline void	z_constraint(t_context *ctx)
{
	t_vec3		right;
	t_vec3		up;
	float		right_align;
	float		up_align;

	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_SHIFT))
	{
		right = ctx->scene.cam.right;
		right.z = 0.0f;
		ctx->editor.axis_primary = vec3_normalize(right);
		up = ctx->scene.cam.up;
		up.z = 0.0f;
		ctx->editor.axis_secondary = vec3_normalize(up);
	}
	else
	{
		right_align = vec3_dot(ctx->scene.cam.right, g_forward);
		up_align = vec3_dot(ctx->scene.cam.up, g_forward);
		ctx->editor.axis_primary = vec3_scale(g_forward, right_align);
		ctx->editor.axis_secondary = vec3_scale(g_forward, up_align);
	}
}
