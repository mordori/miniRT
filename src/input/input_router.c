#include "camera.h"
#include "editing.h"
#include "input.h"
#include "rendering.h"

static inline void flag_update(t_context* ctx, bool* update);

void process_input(t_context* ctx, bool* update) {
	if (ctx->renderer.mode != RENDERED)
		while (ctx->renderer.threads_running)
			pthread_cond_wait(&ctx->renderer.cond, &ctx->renderer.mutex);

	t_vec2i mouseDelta = get_mouse_delta(ctx);
	bool dirty = false;
	dirty |= config_camera(ctx);
	dirty |= control_camera(ctx, mouseDelta);
	dirty |= edit_object(ctx, mouseDelta);
	if (ctx->renderer.mode != SOLID)
		dirty |= cam_fly(ctx);
	dirty |= rotate_skydome(ctx);
	if (dirty)
		flag_update(ctx, update);
}

static inline void flag_update(t_context* ctx, bool* update) {
	*update = true;
	update_camera(ctx, &ctx->scene.cam);
	if (ctx->renderer.mode == RENDERED)
		atomic_store(&ctx->renderer.render_cancel, true);
}

void key_hook(mlx_key_data_t keydata, void* param) {
	t_context* ctx = (t_context*)param;

	pthread_mutex_lock(&ctx->renderer.mutex);
	bool dirty = false;
	if (ctx->renderer.mode == SOLID) {
		while (ctx->renderer.threads_running)
			pthread_cond_wait(&ctx->renderer.cond, &ctx->renderer.mutex);

		dirty |= config_editor(ctx, keydata);
		if (ctx->editor.mode == EDIT_DEFAULT && keydata.key == MLX_KEY_F && keydata.action == MLX_PRESS)
			dirty |= frame_camera(ctx, ctx->editor.selected_obj);
		if (keydata.key == MLX_KEY_Q && keydata.action == MLX_PRESS)
			dirty |= deselect_object(ctx, &ctx->renderer);
	}
	if (keydata.key == MLX_KEY_H && keydata.action == MLX_PRESS)
		ctx->hide_stats = !ctx->hide_stats;
	if (ctx->renderer.mode != SOLID && keydata.key == MLX_KEY_R && keydata.action == MLX_PRESS)
		dirty |= reset_camera(ctx);
	pthread_mutex_unlock(&ctx->renderer.mutex);

	if (ctx->renderer.mode != SOLID && keydata.key == MLX_KEY_T && keydata.action == MLX_PRESS)
		set_default_view(ctx);
	dirty |= config_renderer(ctx, keydata);

	if (ctx->renderer.mode != SOLID && keydata.key == MLX_KEY_Y && keydata.action == MLX_PRESS)
		screenshot(ctx);

	if (dirty)
		atomic_store(&ctx->renderer.render_cancel, true);
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
		mlx_close_window(ctx->mlx);
}

void mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void* param) {
	mods = 0;
	t_context* ctx = (t_context*)param;
	t_renderer* r = &ctx->renderer;

	pthread_mutex_lock(&r->mutex);
	if (r->mode == SOLID) {
		while (r->threads_running)
			pthread_cond_wait(&r->cond, &r->mutex);
		if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS && r->cam.state == CAM_DEFAULT &&
			!mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT)) {
			if (ctx->editor.mode == EDIT_DEFAULT)
				select_object(ctx);
			else
				apply_edit_action(ctx);
			mods = 1;
		} else if (button == MLX_MOUSE_BUTTON_RIGHT && action == MLX_PRESS && ctx->editor.mode != EDIT_DEFAULT &&
			!mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT)) {
			mods = cancel_edit_action(ctx);
		}
	}
	if (mods)
		atomic_store(&ctx->renderer.render_cancel, true);
	pthread_mutex_unlock(&r->mutex);
}
