#include "rendering.h"

static inline void	*render_thread(void *arg);

bool	init_threads(t_context *ctx)
{
	ctx->render_active = true;
	ctx->render_paused = true;
	while (ctx->threads_init < ctx->threads_target)
	{
		if (!pthread_create(&ctx->threads[ctx->threads_init], NULL, render_thread, ctx))
			break ;
		++ctx->threads_init;
	}
	return (ctx->threads_init == ctx->threads_target);
}

static inline void	*render_thread(void *arg)
{
	t_context	*ctx;

	ctx = (t_context *)arg;
	while (ctx->render_active)
	{
		while (ctx->render_paused)
			usleep(SPIN_TIME);
		get_render_bucket();
		render();
	}
	return (NULL);
}
