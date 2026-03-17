#include "camera.h"
#include "input.h"

static inline void	cam_look(t_context *ctx, t_vec2i delta);

void	apply_cam_action(t_context *ctx, t_vec2i delta)
{
	t_camera	*cam;

	cam = &ctx->scene.cam;
	if (delta.x > -300 && delta.x < 300 && delta.y > -300 && delta.y < 300)
	{
		if (delta.x || delta.y)
		{
			if (cam->state == CAM_ORBIT)
				cam_look(ctx, delta);
			else if (cam->state == CAM_ZOOM)
				cam_look(ctx, delta);
			else if (cam->state == CAM_PAN)
				cam_look(ctx, delta);
			else if (cam->state == CAM_LOOK)
				cam_look(ctx, delta);
		}
	}
}

static inline void	cam_look(t_context *ctx, t_vec2i delta)
{
	float		speed;

	speed = ctx->mlx->delta_time * 60.0f * SENS_ORBIT * 14.0f / ctx->renderer.cam.focal_len_mm;
	ctx->scene.cam.yaw += delta.x * speed;
	ctx->scene.cam.pitch -= delta.y * speed;
}
