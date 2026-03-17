#include "input.h"

t_vec2i	get_mouse_delta(t_context *ctx)
{
	t_vec2i		delta;

	mlx_get_mouse_pos(ctx->mlx, &ctx->mouse.pos.x, &ctx->mouse.pos.y);
	if (ctx->mouse.pos.x >= (int32_t)ctx->img->width - 1)
		mlx_set_mouse_pos(ctx->mlx, 1, ctx->mouse.pos.y);
	else if (ctx->mouse.pos.x <= 0)
		mlx_set_mouse_pos(ctx->mlx, (int32_t)ctx->img->width - 2, ctx->mouse.pos.y);
	if (ctx->mouse.pos.y >= (int32_t)ctx->img->height - 1)
		mlx_set_mouse_pos(ctx->mlx, ctx->mouse.pos.x, 1);
	else if (ctx->mouse.pos.x <= 0)
		mlx_set_mouse_pos(ctx->mlx, ctx->mouse.pos.x, (int32_t)ctx->img->height - 2);
	delta = vec2i_sub(ctx->mouse.pos, ctx->mouse.pos_prev);
	if (delta.x > (int32_t)ctx->img->width / 2)
		delta.x -= (ctx->img->width - 2);
	else if (delta.x < -(int32_t)ctx->img->width / 2)
		delta.x += (ctx->img->width - 2);
	if (delta.y > (int32_t)ctx->img->height / 2)
		delta.y -= (ctx->img->height - 2);
	else if (delta.y < -(int32_t)ctx->img->height / 2)
		delta.y += (ctx->img->height - 2);
	return (delta);
}
