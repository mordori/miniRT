#include "camera.h"

static inline t_vec3 get_key_input(mlx_t* mlx);
static inline t_vec3 compute_move_vec(t_camera* cam, t_vec3 input, float dt);
static inline void clamp_camera(t_camera* cam);

bool camera_movement(t_context* ctx) {
	if (ctx->renderer.mode == SOLID)
		return false;

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
