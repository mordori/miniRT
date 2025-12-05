#include "camera.h"

void	add_camera(t_context *ctx, char **params)
{
	t_camera	*cam;

	(void)params;
	cam = &ctx->scene.cam;
	cam->state = CAM_DEFAULT;
	// cam->transform.pos = ;
	// cam->target.pos = vec3_add(cam->transform.pos, vec3(x, y, z));
	// cam->fov = ;
	//init viewport
	update_cam(ctx);
}

void	update_cam(t_context *ctx)
{
	const float	limit = M_PI_2 - 0.001f;
	t_vec3		dir;
	t_camera	*cam;

	cam = &ctx->scene.cam;
	if (ctx->selected_object)
		cam->pivot = ctx->selected_object->transform.pos;
	else
		cam->pivot = (t_vec3){0};
	// add relative deltas
	if (cam->pitch > limit)
		cam->pitch = limit;
	if (cam->pitch < -limit)
		cam->pitch = -limit;
	dir.x = cosf(cam->pitch) * sinf(cam->yaw);
	dir.y = sinf(cam->pitch);
	dir.z = cosf(cam->pitch) * cosf(cam->yaw);
	cam->transform.pos = vec3_add(cam->target.pos, vec3_scale(dir, cam->distance));
	ctx->scene.sky_sphere.transform.pos = ctx->scene.cam.transform.pos;
}

// void	compute_distance(t_context *ctx)
// {
// 	const float	padding = 1.5f;
// 	float		max_dim;
// 	t_camera	*cam;

// 	cam = &ctx->scene.cam;
// 	max_dim = fmaxf(fmaxf(ctx->bounds.x, ctx->bounds.y), ctx->bounds.z);
// 	if (ctx->img->width < ctx->img->height)
// 		max_dim /= cam->aspect;
// 	max_dim *= padding;
// 	cam->distance = (max_dim * 0.5f) / tanf(cam->fov * 0.5f);
// }
