#include "camera.h"
#include "input.h"

static inline void	cam_look(t_context *ctx, t_vec2i delta);
// static inline void	cam_orbit(t_context *ctx, t_vec2i delta);
// static inline void	cam_zoom(t_context *ctx, t_vec2i delta);
// static inline void	cam_pan(t_context *ctx, t_vec2i delta);

void	apply_cam_action(t_context *ctx, t_vec2i delta)
{
	t_camera	*cam;

	cam = &ctx->scene.cam;
	if (delta.x > -300 && delta.x < 300 && delta.y > -300 && delta.y < 300)
	{
		if (delta.x || delta.y)
		{
			if (cam->state == CAM_LOOK)
				cam_look(ctx, delta);
			else if (cam->state == CAM_ORBIT)
				cam_look(ctx, delta);
			// else if (cam->state == CAM_ZOOM)
			// 	cam_zoom(ctx, delta);
			// else if (cam->state == CAM_PAN)
			// 	cam_pan(ctx, delta);
		}
	}
}

static inline void	cam_look(t_context *ctx, t_vec2i delta)
{
	float		speed;

	speed = ctx->mlx->delta_time * 60.0f * SENS_ORBIT / ctx->renderer.cam.focal_len_mm;
	ctx->scene.cam.yaw += (float)delta.x * speed;
	ctx->scene.cam.pitch -= (float)delta.y * speed;
}

// static inline void	cam_orbit(t_context *ctx, t_vec2i delta)
// {

// }

// static inline void	cam_zoom(t_context *ctx, t_vec2i delta)
// {

// }

// static inline void	cam_pan(t_context *ctx, t_vec2i delta)
// {

// }
