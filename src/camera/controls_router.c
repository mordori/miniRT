#include "camera.h"
#include "input.h"

static inline bool	set_cam_state(t_context *ctx);
static inline void	begin_cam_action(t_context *ctx, t_cam_state state);
static inline void	end_cam_action(t_context *ctx);
static inline bool	is_cam_action_active(const t_context *ctx);

bool	control_camera(t_context *ctx)
{
	t_vec2i		delta;

	if (ctx->editor.mode != EDIT_DEFAULT || !set_cam_state(ctx))
		return (false);
	if (!is_cam_action_active(ctx))
	{
		end_cam_action(ctx);
		return (true);
	}
	delta = get_mouse_delta(ctx);
	apply_cam_action(ctx, delta);
	ctx->mouse.pos_prev = ctx->mouse.pos;
	return (delta.x != 0 || delta.y != 0);
}

static inline bool	set_cam_state(t_context *ctx)
{
	if (ctx->scene.cam.state != CAM_DEFAULT)
		return (true);
	if (ctx->renderer.mode == SOLID && mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT))
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
		if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
			begin_cam_action(ctx, CAM_LOOK);
		else
			return (false);
	}
	else
		return (false);
	return (true);
}

static inline void	begin_cam_action(t_context *ctx, t_cam_state state)
{
	ctx->scene.cam.state = state;
	mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
	mlx_get_mouse_pos(ctx->mlx, &ctx->mouse.pos_orig.x, &ctx->mouse.pos_orig.y);
	ctx->mouse.pos = ctx->mouse.pos_orig;
	ctx->mouse.pos_prev = ctx->mouse.pos_orig;
}

static inline void	end_cam_action(t_context *ctx)
{
	ctx->scene.cam.state = CAM_DEFAULT;
	mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_NORMAL);
	mlx_set_mouse_pos(ctx->mlx, ctx->mouse.pos_orig.x, ctx->mouse.pos_orig.y);
	ctx->mouse.pos = ctx->mouse.pos_orig;
	ctx->mouse.pos_prev = ctx->mouse.pos_orig;
}

static inline bool	is_cam_action_active(const t_context *ctx)
{
	const t_camera	*cam = &ctx->scene.cam;
	bool			is_held;

	is_held = false;
	if (cam->state == CAM_ORBIT && mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
		is_held = true;
	else if (cam->state == CAM_ZOOM && mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
		is_held = true;
	else if (cam->state == CAM_PAN && mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
		is_held = true;
	else if (cam->state == CAM_LOOK && mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
		is_held = true;
	return (is_held);
}
