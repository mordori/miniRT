#include "camera.h"

static inline bool set_f_stop(t_context* ctx, float dt);
static inline bool set_focal_length(t_context* ctx, float dt);
static inline bool set_focus_dist(t_context* ctx, float dt);

bool config_camera(t_context* ctx) {
	float dt = fminf(ctx->mlx->delta_time, 0.1f);
	bool dirty = false;
	dirty |= set_f_stop(ctx, dt);
	dirty |= set_focal_length(ctx, dt);
	dirty |= set_focus_dist(ctx, dt);
	return dirty;
}

static inline bool set_f_stop(t_context* ctx, float dt) {
	int change = mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT) - mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT);
	if (change) {
		float mul = powf(2.0f, 0.15f * dt);
		float div = 1.0f / mul;
		if (change > 0)
			ctx->scene.cam.f_stop *= mul;
		else
			ctx->scene.cam.f_stop *= div;
		ctx->scene.cam.f_stop = clampf(ctx->scene.cam.f_stop, 0.95f, 128.0f);
		return true;
	}
	return false;
}

static inline bool set_focal_length(t_context* ctx, float dt) {
	int change = mlx_is_key_down(ctx->mlx, MLX_KEY_L) - mlx_is_key_down(ctx->mlx, MLX_KEY_K);
	if (change) {
		float mul = powf(2.0f, 0.5f * dt);
		float div = 1.0f / mul;
		if (change > 0)
			ctx->scene.cam.focal_len_mm *= mul;
		else
			ctx->scene.cam.focal_len_mm *= div;
		ctx->scene.cam.focal_len_mm = clampf(ctx->scene.cam.focal_len_mm, 1.0f, 800.0f);
		return true;
	}
	return false;
}

static inline bool set_focus_dist(t_context* ctx, float dt) {
	int change = mlx_is_key_down(ctx->mlx, MLX_KEY_UP) - mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN);
	if (change) {
		float mul = powf(2.0f, 0.3f * dt);
		float div = 1.0f / mul;
		if (change > 0)
			ctx->scene.cam.focus_dist *= mul;
		else
			ctx->scene.cam.focus_dist *= div;
		ctx->scene.cam.focus_dist = clampf(ctx->scene.cam.focus_dist, 0.1f, 1000.0f);
		return true;
	}
	return false;
}
