#include "editing.h"
#include "utils.h"

void	config_editor(t_context *ctx, mlx_key_data_t keydata)
{
	if (keydata.action == MLX_RELEASE)
	{
		if (keydata.key == MLX_KEY_W)
			ctx->editor.mode = EDIT_TRANSLATE;
		else if (keydata.key == MLX_KEY_E)
			ctx->editor.mode = EDIT_ROTATE;
		else if (keydata.key == MLX_KEY_R)
			ctx->editor.mode = EDIT_ROTATE;
		if (ctx->editor.mode != EDIT_DEFAULT)
		{
			if (keydata.key == MLX_KEY_X)
			{
				ctx->editor.constraints = AXIS_X;
				if (keydata.modifier == MLX_SHIFT)
					ctx->editor.constraints = AXIS_YZ;
			}
			else if (keydata.key == MLX_KEY_Y)
			{
				ctx->editor.constraints = AXIS_Y;
				if (keydata.modifier == MLX_SHIFT)
					ctx->editor.constraints = AXIS_XZ;
			}
			else if (keydata.key == MLX_KEY_Z)
			{
				ctx->editor.constraints = AXIS_Z;
				if (keydata.modifier == MLX_SHIFT)
					ctx->editor.constraints = AXIS_XY;
			}
			else
				ctx->editor.constraints = AXIS_DEFAULT;
		}
	}
}

bool	edit(t_context *ctx)
{
	if (ctx->scene.cam.state != CAM_DEFAULT)
		return (false);
	(void)ctx;
	return (false);
}

void	apply_edit(t_context *ctx)
{
	ctx->editor.mode = EDIT_DEFAULT;
}

void	cancel_edit(t_context *ctx)
{
	ctx->editor.mode = EDIT_DEFAULT;
}
