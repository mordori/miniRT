#include "camera.h"

#include "rendering.h"
#include "utils.h"

static inline void update_viewport(t_camera* cam, t_viewport* vp, float img_width, float img_height);

void init_camera(t_context* ctx, t_vec3 position, t_vec3 orientation, float fov) {
	if (!ctx)
		return;

	t_camera* cam = &ctx->scene.cam;
	cam->state = CAM_DEFAULT;
	cam->transform.pos = position;
	cam->yaw = atan2f(orientation.x, orientation.z);
	cam->pitch = asinf(orientation.y);
	cam->transform.rot = quat_from_euler(vec3(-cam->pitch, cam->yaw, 0.0f));
	cam->distance = fmaxf(vec3_dist(cam->transform.pos, g_zero), 0.01f);
	fov = clampf(degrees_to_rad(fov), degrees_to_rad(0.01f), degrees_to_rad(179.9f));
	cam->focal_len_mm = SENSOR_HALF_HEIGHT_MM / tanf(fov * 0.5f);
	cam->focal_len_mm = clampf(cam->focal_len_mm, 1.0f, 800.0f);
	cam->focus_dist = clampf(cam->focus_dist, 0.1f, 1000.0f);
	cam->f_stop = 16.0f;
	cam->shutter_speed = 1.0f / 100.0f;
	cam->iso = 100.0f;
	cam->init_pos = position;
	cam->init_rot = cam->transform.rot;
	cam->init_focal_len_mm = cam->focal_len_mm;
	cam->init_focus_dist = cam->focus_dist;
}

t_vec3 sample_defocus_disk(const t_context* ctx, t_pixel* pixel) {
	t_vec2 uv = r4_sequence_d12(
		pixel->frame, vec2(static_blue_noise(&ctx->tex_bn, pixel, BN_DD_U), static_blue_noise(&ctx->tex_bn, pixel, BN_DD_V)));
	t_vec2 disk = sample_disk(uv);
	t_vec3 u = vec3_scale(ctx->renderer.cam.defocus_disk_u, disk.x);
	t_vec3 v = vec3_scale(ctx->renderer.cam.defocus_disk_v, disk.y);
	return vec3_add(ctx->renderer.cam.transform.pos, vec3_add(u, v));
}

void update_camera(t_context* ctx, t_camera* cam) {
	cam->distance = clampf(cam->distance, 0.01f, WORLD_LIMIT);
	cam->transform.pos = vec3_clamp(cam->transform.pos, -WORLD_LIMIT, WORLD_LIMIT);
	cam->transform.rot = quat_normalize(cam->transform.rot);
	t_mat4 m = quat_to_mat4(cam->transform.rot);
	cam->right = vec3_normalize(vec3(m.m[0][0], m.m[1][0], m.m[2][0]));
	cam->up = vec3_normalize(vec3(m.m[0][1], m.m[1][1], m.m[2][1]));
	cam->forward = vec3_normalize(vec3(m.m[0][2], m.m[1][2], m.m[2][2]));
	cam->aspect = (float)ctx->img->width / ctx->img->height;
	update_viewport(cam, &cam->viewport, (float)ctx->img->width, (float)ctx->img->height);
}

static inline void update_viewport(t_camera* cam, t_viewport* vp, float img_width, float img_height) {
	vp = (t_viewport*)&cam->viewport;
	vp->height = SENSOR_HEIGHT_MM / cam->focal_len_mm;
	vp->width = vp->height * cam->aspect;
	t_vec3 u = vec3_scale(cam->right, vp->width);
	t_vec3 v = vec3_scale(cam->up, -vp->height);
	vp->d_u = vec3_div(u, img_width);
	vp->d_v = vec3_div(v, img_height);
	t_vec3 vp_center = vec3_add(cam->transform.pos, cam->forward);
	t_vec3 vp_up_left = vec3_sub(vp_center, vec3_scale(u, 0.5f));
	vp_up_left = vec3_sub(vp_up_left, vec3_scale(v, 0.5f));
	vp->pixel_00_loc = vp_up_left;
	float defocus_r = cam->focal_len_mm * 0.001f / (2.0f * cam->f_stop);
	cam->defocus_disk_u = vec3_scale(cam->right, defocus_r);
	cam->defocus_disk_v = vec3_scale(cam->up, defocus_r);
}
