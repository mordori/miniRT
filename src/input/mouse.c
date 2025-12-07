#include "input.h"

/**
 * Wraps the mouse horizontally across the screen boundaries.
 *
 * If the mouse reaches the right or left edge of the window, it is
 * repositioned to the opposite side. The number of wraps is tracked internally.
 *
 * @param ctx Rendering context containing mlx context and render image.
 * @param pos Current mouse screen position.
 * @return The total horizontal displacement: wraps × (window width - 2)
 */
int	wrap_mouse_x(t_context *ctx, t_vec2i *pos)
{
	static int32_t	wraps_x = {0};

	if (pos->x >= (int32_t)ctx->img->width - 1)
	{
		pos->x = 1;
		mlx_set_mouse_pos(ctx->mlx, pos->x, pos->y);
		++wraps_x;
	}
	else if (pos->x <= 0)
	{
		pos->x = ctx->img->width - 2;
		mlx_set_mouse_pos(ctx->mlx, pos->x, pos->y);
		--wraps_x;
	}
	return (wraps_x * (ctx->img->width - 2));
}

/**
 * Wraps the mouse vertically across the screen boundaries.
 *
 * If the mouse reaches the top or bottom edge of the window, it is
 * repositioned to the opposite side. The number of wraps is tracked internally.
 *
 * @param ctx Rendering context containing mlx context and render image.
 * @param pos Current mouse screen position.
 * @return The total vertical displacement: wraps × (window height - 2).
 */
int	wrap_mouse_y(t_context *ctx, t_vec2i *pos)
{
	static int32_t	wraps_y = {0};

	if (pos->y >= (int32_t)ctx->img->height - 1)
	{
		pos->y = 1;
		mlx_set_mouse_pos(ctx->mlx, pos->x, pos->y);
		++wraps_y;
	}
	else if (pos->y <= 0)
	{
		pos->y = ctx->img->height - 2;
		mlx_set_mouse_pos(ctx->mlx, pos->x, pos->y);
		--wraps_y;
	}
	return (wraps_y * (ctx->img->height - 2));
}
