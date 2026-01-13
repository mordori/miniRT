#include "camera.h"
#include "input.h"

static inline void	set_cam_state(t_context *ctx, t_vec2i *original_pos, t_vec2i *previous_pos);
static inline void	orbit(t_context *ctx, t_vec2i pos, t_vec2i *previous_pos, t_vec2i original_pos);
static inline void	zoom(t_context *ctx, t_vec2i pos, t_vec2i *previous_pos, t_vec2i original_pos);
static inline void	pan(t_context *ctx, t_vec2i pos, t_vec2i *previous_pos);

bool	control_camera(t_context *ctx)
{
	static t_vec2i	logical_mouse;
	static t_vec2i	original_pos;
	static t_vec2i	previous_pos;
	t_vec2i			pos;
	t_camera		*cam;

	cam = &ctx->scene.cam;
	set_cam_state(ctx, &original_pos, &previous_pos);
	if (cam->state == CAM_DEFAULT)
	{
		mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_NORMAL);
		return (false);
	}
	mlx_get_mouse_pos(ctx->mlx, &pos.x, &pos.y);
	if (cam->state != CAM_PAN)
		logical_mouse = vec2i(wrap_mouse_x(ctx, &pos), wrap_mouse_y(ctx, &pos));
	pos = vec2i_add(pos, logical_mouse);
	if (cam->state == CAM_ORBIT)
		orbit(ctx, pos, &previous_pos, original_pos);
	else if (cam->state == CAM_ZOOM)
		zoom(ctx, pos, &previous_pos, original_pos);
	else
		pan(ctx, pos, &previous_pos);
	return (true);
}

static inline void	set_cam_state(t_context *ctx, t_vec2i *original_pos, t_vec2i *previous_pos)
{
	t_camera	*cam;

	cam = &ctx->scene.cam;
	if (cam->state == CAM_DEFAULT && mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT))
	{
		if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
			cam->state = CAM_ORBIT;
		else if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
			cam->state = CAM_ZOOM;
		else if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
			cam->state = CAM_PAN;
		if (cam->state == CAM_DEFAULT)
			return ;
		if (cam->state != CAM_PAN)
			mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
		mlx_get_mouse_pos(ctx->mlx, &original_pos->x, &original_pos->y);
		*previous_pos = *original_pos;
	}
}

static inline void	orbit(t_context *ctx, t_vec2i pos, t_vec2i *previous_pos, t_vec2i original_pos)
{
	float		speed;
	t_vec2i		delta;
	static bool	init;

	if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
	{
		if (init)
		{
			delta = vec2i_sub(pos, *previous_pos);
			if (delta.x || delta.y)
			{
				speed = ctx->mlx->delta_time * 60.0f * SENS_ORBIT;
				ctx->scene.cam.yaw += delta.x * speed;
				ctx->scene.cam.pitch -= delta.y * speed;
			}
		}
		*previous_pos = pos;
		init = true;
	}
	else
	{
		ctx->scene.cam.state = CAM_DEFAULT;
		mlx_set_mouse_pos(ctx->mlx, original_pos.x, original_pos.y);
		init = false;
	}
}

static inline void	zoom(t_context *ctx, t_vec2i pos, t_vec2i *previous_pos, t_vec2i original_pos)
{
	float		speed;
	t_vec2i		delta;

	if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{
		delta = vec2i_sub(pos, *previous_pos);
		if (delta.x || delta.y)
		{
			speed = ctx->scene.cam.distance * ctx->mlx->delta_time * 60.0f * SENS_ZOOM;
			ctx->scene.cam.distance = fmaxf(0.1f, ctx->scene.cam.distance - delta.y * speed);
		}
		*previous_pos = pos;
	}
	else
	{
		ctx->scene.cam.state = CAM_DEFAULT;
		mlx_set_mouse_pos(ctx->mlx, original_pos.x, original_pos.y);
	}
}

static inline void	pan(t_context *ctx, t_vec2i pos, t_vec2i *previous_pos)
{
	float		speed;
	t_vec2i		delta;

	if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
	{
		delta = vec2i_sub(pos, *previous_pos);
		if (delta.x || delta.y)
		{
			speed = fmaxf(ctx->scene.cam.distance, 1.0f) * (ctx->scene.cam.fov / (M_PI / 2.5f));
			speed *= ctx->mlx->delta_time * 60.0f * SENS_PAN;
		}
		*previous_pos = pos;
		(void)speed;
	}
	else
		ctx->scene.cam.state = CAM_DEFAULT;
}
