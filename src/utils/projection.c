#include "utils.h"

t_vec2	world_to_screen(t_context *ctx, t_vec3 pos)
{
	const t_camera		*cam = &ctx->scene.cam;
	const t_viewport	*vp = &cam->viewport;
	t_vec3				cam_to_obj;
	float				depth;
	t_vec3				hit_plane;
	t_vec3				offset;
	t_vec2				screen_pos;

	cam_to_obj = vec3_sub(pos, cam->transform.pos);
	depth = vec3_dot(cam->forward, cam_to_obj);
	if (depth <= 0.001f)
		return (vec2(ctx->img->width * 0.5f, ctx->img->height * 0.5f));
	hit_plane = vec3_add(cam->transform.pos, vec3_scale(cam_to_obj, 1.0f / depth));
	offset = vec3_sub(hit_plane, vp->pixel_00_loc);
	screen_pos.x = vec3_dot(offset, vp->d_u) / vec3_dot(vp->d_u, vp->d_u);
	screen_pos.y = vec3_dot(offset, vp->d_v) / vec3_dot(vp->d_v, vp->d_v);
	return (screen_pos);
}
