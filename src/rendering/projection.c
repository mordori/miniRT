#include "rendering.h"

t_vec2 world_to_screen(t_context* ctx, const t_camera* cam, const t_viewport* vp, t_vec3 pos) {
	t_vec3 cam_to_obj = vec3_sub(pos, cam->transform.pos);
	float depth = vec3_dot(cam->forward, cam_to_obj);
	if (depth <= 0.001f)
		return vec2(ctx->img->width * 0.5f, ctx->img->height * 0.5f);

	t_vec3 hit_plane = vec3_add(cam->transform.pos, vec3_scale(cam_to_obj, 1.0f / depth));
	t_vec3 offset = vec3_sub(hit_plane, vp->pixel_00_loc);
	return (t_vec2){ //
		.x = vec3_dot(offset, vp->d_u) / vec3_dot(vp->d_u, vp->d_u),
		.y = vec3_dot(offset, vp->d_v) / vec3_dot(vp->d_v, vp->d_v)
	};
}
