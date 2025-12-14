#include "utils.h"
#include "camera.h"
#include "rendering.h"

void	resize_window(t_context *ctx)
{
	t_renderer	*r;

	if (!ctx || !ctx->mlx || !ctx->img)
		return ;
	r = &ctx->renderer;
	if (r->buffer)
		free(r->buffer);
	r->buffer = malloc(sizeof(t_vec3) * (r->width * r->height));
	if (!r->buffer || !mlx_resize_image(ctx->img, r->width, r->height))
		fatal_error(ctx, errors(ERR_RESIZE), __FILE__, __LINE__);
	*r->buffer = (t_vec3){0};
	update_camera(ctx);
	if (r->threads_init == r->threads_amount)
		start_render(r);
	atomic_store(&r->resize_pending, false);
}
