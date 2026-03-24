#include "camera.h"
#include "input.h"

static inline bool	set_cam_state(t_context *ctx);

bool	control_camera(t_context *ctx, t_vec2i delta)
{
	if (ctx->editor.mode != EDIT_DEFAULT || !set_cam_state(ctx))
		return (false);
	if (!is_cam_action_active(ctx))
	{
		end_cam_action(ctx);
		return (true);
	}
	apply_cam_action(ctx, delta);
	return (true);
}

static inline bool	set_cam_state(t_context *ctx)
{
	if (ctx->scene.cam.state != CAM_DEFAULT)
		return (true);
	if (ctx->renderer.mode == SOLID && \
mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT))
	{
		if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
			begin_cam_action(ctx, CAM_ORBIT);
		else if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
			begin_cam_action(ctx, CAM_ZOOM);
		else if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
			begin_cam_action(ctx, CAM_PAN);
		else
			return (false);
	}
	else if (ctx->renderer.mode != SOLID)
	{
		if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
			begin_cam_action(ctx, CAM_TURN);
		else if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
			begin_cam_action(ctx, CAM_LOOK);
		else
			return (false);
	}
	else
		return (false);
	return (true);
}
