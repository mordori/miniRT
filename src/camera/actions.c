#include "camera.h"
#include "defines.h"

void apply_cam_action(t_context* ctx, t_vec2i delta) {
	t_camera* cam = &ctx->scene.cam;
	switch (cam->state) {
		case CAM_TURN: cam_turn(ctx, delta); break;
		case CAM_ORBIT: cam_orbit(ctx, delta); break;
		case CAM_ZOOM: cam_zoom(ctx, delta); break;
		case CAM_PAN: cam_pan(ctx, delta); break;
		case CAM_DEFAULT: return;
	}
}

void begin_cam_action(t_context* ctx, t_cam_state state) {
	ctx->scene.cam.state = state;
	mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
	mlx_get_mouse_pos(ctx->mlx, &ctx->mouse.pos_orig.x, &ctx->mouse.pos_orig.y);
	ctx->mouse.pos = ctx->mouse.pos_orig;
	ctx->mouse.pos_prev = ctx->mouse.pos_orig;
}

void end_cam_action(t_context* ctx) {
	ctx->scene.cam.state = CAM_DEFAULT;
	mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_NORMAL);
	mlx_set_mouse_pos(ctx->mlx, ctx->mouse.pos_orig.x, ctx->mouse.pos_orig.y);
	ctx->mouse.pos = ctx->mouse.pos_orig;
	ctx->mouse.pos_prev = ctx->mouse.pos_orig;
}

bool is_cam_action_active(const t_context* ctx) {
	const t_camera* cam = &ctx->scene.cam;
	mouse_key_t key;
	switch (cam->state) {
		case CAM_ORBIT: key = MLX_MOUSE_BUTTON_LEFT; break;
		case CAM_ZOOM: key = MLX_MOUSE_BUTTON_RIGHT; break;
		case CAM_PAN: key = MLX_MOUSE_BUTTON_MIDDLE; break;
		case CAM_TURN: key = MLX_MOUSE_BUTTON_RIGHT; break;
		case CAM_DEFAULT: return false;
	}
	return mlx_is_mouse_down(ctx->mlx, key);
}
