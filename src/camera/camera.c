#include "camera.h"

void	add_camera(t_context *ctx, char *src)
{
	t_camera	*cam;

	*cam = ctx->scene.cam;
	// cam->transform.pos = ;
	// cam->target.pos = vec3_add(cam->transform.pos, vec3(x, y, z));
	// cam->fov = ;
}

void	update_cam(t_context *ctx)
{
	ctx->scene.sky_sphere.transform.pos = ctx->scene.cam.transform.pos;
}
