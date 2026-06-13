#include "camera.h"

void cam_look(t_context* ctx, t_vec2i delta) {
	t_camera* cam = &ctx->scene.cam;
	float speed = SENS_LOOK * (14.0f / cam->focal_len_mm);
	cam->yaw += (float)delta.x * speed;
	cam->pitch -= (float)delta.y * speed;
	cam->pitch = clampf(cam->pitch, -M_PI_2f + 0.001f, M_PI_2f - 0.001f);
	cam->transform.rot = quat_from_euler(vec3(-cam->pitch, cam->yaw, 0.0f));
	update_camera(ctx, cam);
}

void cam_turn(t_context* ctx, t_vec2i delta) {
	cam_look(ctx, delta);
	ctx->scene.cam.control_forward = ctx->scene.cam.forward;
	ctx->scene.cam.control_right = ctx->scene.cam.right;
}

void cam_orbit(t_context* ctx, t_vec2i delta) {
	t_camera* cam = &ctx->scene.cam;
	if (ctx->editor.selected_obj)
		cam->target = ctx->editor.selected_obj->transform.pos;
	else
		cam->target = vec3_add(cam->transform.pos, vec3_scale(cam->forward, cam->distance));
	t_vec3 diff = vec3_sub(cam->target, cam->transform.pos);
	cam->distance = vec3_length(diff);
	t_vec3 local = vec3(vec3_dot(diff, cam->right), vec3_dot(diff, cam->up), vec3_dot(diff, cam->forward));
	cam->yaw += (float)delta.x * SENS_ORBIT;
	cam->pitch -= (float)delta.y * SENS_ORBIT;
	cam->pitch = clampf(cam->pitch, -M_PI_2f + 0.001f, M_PI_2f - 0.001f);
	cam->transform.rot = quat_from_euler(vec3(-cam->pitch, cam->yaw, 0.0f));
	update_camera(ctx, cam);
	diff = vec3_add(vec3_scale(cam->right, local.x), vec3_scale(cam->up, local.y));
	diff = vec3_add(diff, vec3_scale(cam->forward, local.z));
	cam->transform.pos = vec3_sub(cam->target, diff);
	ctx->scene.cam.control_forward = ctx->scene.cam.forward;
	ctx->scene.cam.control_right = ctx->scene.cam.right;
}

void cam_zoom(t_context* ctx, t_vec2i delta) {
	t_camera* cam = &ctx->scene.cam;
	float move = (float)delta.y * SENS_ZOOM * cam->distance;
	cam->transform.pos = vec3_add(cam->transform.pos, vec3_scale(cam->forward, move));
	cam->distance = cam->distance - move;
}

void cam_pan(t_context* ctx, t_vec2i delta) {
	t_camera* cam = &ctx->scene.cam;
	float speed = SENS_PAN * cam->distance;
	t_vec3 move = vec3_add(vec3_scale(cam->right, -(float)delta.x * speed), vec3_scale(cam->up, (float)delta.y * speed));
	cam->transform.pos = vec3_add(cam->transform.pos, move);
}
