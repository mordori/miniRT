#include "editing.h"
#include "utils.h"

static inline void	set_edit_mode(t_context *ctx, t_vec2i *original_pos,
		t_vec2i *previous_pos);

void	config_editor(t_context *ctx, mlx_key_data_t keydata)
{
	(void)ctx;
	(void)keydata;
}

bool	edit(t_context *ctx)
{
	// static t_vec2i	logical_mouse;
	static t_vec2i	original_pos;
	static t_vec2i	previous_pos;
	// t_vec2i			pos;

	if (!ctx->editor.selected_obj)
		return (false);
	set_edit_mode(ctx, &original_pos, &previous_pos);
	if (ctx->editor.mode == EDIT_DEFAULT)
		mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_NORMAL);
	// update_transform(&ctx->editor.selected_obj->transform);
	return (true);
}

void	apply_edit(t_context *ctx)
{
	ctx->editor.mode = EDIT_DEFAULT;
}

void	cancel_edit(t_context *ctx)
{
	ctx->editor.mode = EDIT_DEFAULT;
}

static inline void	set_edit_mode(t_context *ctx, t_vec2i *original_pos,
		t_vec2i *previous_pos)
{
	if (ctx->editor.mode == EDIT_DEFAULT)
	{
		// if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
			ctx->editor.mode = EDIT_DEFAULT;
		// else if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
		// 	cam->state = CAM_ZOOM;
		// else if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
		// 	cam->state = CAM_PAN;
		if (ctx->editor.mode == EDIT_DEFAULT)
			return ;
		mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
		mlx_get_mouse_pos(ctx->mlx, &original_pos->x, &original_pos->y);
		*previous_pos = *original_pos;
	}
}

// if (\
// (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT) && !mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT)) || \
// (mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT) && !mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT)))
// 	{
// 		mul = powf(2.0f, 0.15f * ctx->mlx->delta_time);
// 		div = 1.0f / mul;
// 		if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT))
// 			ctx->scene.cam.f_stop *= div;
// 		else
// 			ctx->scene.cam.f_stop *= mul;
// 		ctx->scene.cam.f_stop = clampf(ctx->scene.cam.f_stop, 0.95f, 128.0f);
// 		dirty = true;
// 	}
