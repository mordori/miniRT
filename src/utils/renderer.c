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
	atomic_store(&r->resize_pending, false);
	if (r->threads_init == r->threads_amount)
		start_render(r);
}

bool	resize_timer(t_context *ctx)
{
	uint32_t	time;

	time = time_now() - ctx->resize_time;
	return (time > 200);
}

/**
 * @brief Returns the current time in milliseconds.
 *
 * @return Time in milliseconds since Epoch.
 */
uint32_t	time_now(void)
{
	struct timeval	tv;
	uint32_t		time;

	gettimeofday(&tv, NULL);
	time = (uint32_t)tv.tv_sec * 1000 + (uint32_t)tv.tv_usec / 1000;
	return (time);
}
