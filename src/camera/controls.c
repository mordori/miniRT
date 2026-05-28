#include "camera.h"

static inline t_vec3 get_key_input(mlx_t* mlx);
static inline t_vec3 compute_move_vec(t_camera* cam, t_vec3 input, float dt);
static inline void clamp_camera(t_camera* cam);

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

bool cam_fly(t_context* ctx) {
	t_camera* cam = &ctx->scene.cam;
	t_vec3 input = get_key_input(ctx->mlx);
	if (input.x == 0.0f && input.y == 0.0f && input.z == 0.0f)
		return false;

	float dt = fminf(ctx->mlx->delta_time, 0.1f);
	t_vec3 move = compute_move_vec(cam, input, dt);
	cam->transform.pos = vec3_add(cam->transform.pos, move);
	clamp_camera(cam);
	update_camera(ctx, cam);
	return true;
}

static inline void clamp_camera(t_camera* cam) {
	cam->transform.pos.v = _mm_max_ps(cam->transform.pos.v, g_world_limit_neg.v);
	cam->transform.pos.v = _mm_min_ps(cam->transform.pos.v, g_world_limit.v);
}

static inline t_vec3 get_key_input(mlx_t* mlx) {
	t_vec3 input;
	input.x = mlx_is_key_down(mlx, KEY_RIGHT) - mlx_is_key_down(mlx, KEY_LEFT);
	input.y = mlx_is_key_down(mlx, KEY_UP) - mlx_is_key_down(mlx, KEY_DOWN);
	input.z = mlx_is_key_down(mlx, KEY_FORWARD) - mlx_is_key_down(mlx, KEY_BACK);
	return input;
}

static inline t_vec3 compute_move_vec(t_camera* cam, t_vec3 input, float dt) {
	t_vec3 vec = { { 0.0f, input.y, 0.0f } };
	vec = vec3_add(vec, vec3_scale(cam->control_right, input.x));
	vec = vec3_add(vec, vec3_scale(cam->control_forward, input.z));
	return vec3_scale(vec, SENS_MOVE * dt);
}
