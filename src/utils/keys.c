#include "camera.h"
#include "editing.h"
#include "utils.h"

void check_edit_keys(t_context* ctx, mlx_key_data_t keydata, bool* dirty) {
	if (ctx->renderer.mode == SOLID) {
		while (ctx->renderer.threads_running)
			pthread_cond_wait(&ctx->renderer.cond, &ctx->renderer.mutex);

		*dirty |= config_editor(ctx, keydata);
		if (ctx->editor.mode == EDIT_DEFAULT && keydata.key == MLX_KEY_F && keydata.action == MLX_PRESS)
			*dirty |= frame_camera(ctx, ctx->editor.selected_obj);
		if (keydata.key == MLX_KEY_Q && keydata.action == MLX_PRESS)
			*dirty |= deselect_object(ctx, &ctx->renderer);
	}
}
