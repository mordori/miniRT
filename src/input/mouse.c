#include "input.h"

int	wrap_mouse_x(const t_context *ctx, t_vec2i *pos)
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

int	wrap_mouse_y(const t_context *ctx, t_vec2i *pos)
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
