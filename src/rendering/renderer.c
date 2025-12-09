#include "rendering.h"
#include "utils.h"

static inline void	*worker(void *arg);

bool	init_renderer(t_context *ctx)
{
	t_renderer	*r;

	r = &ctx->renderer;
	r->threads_amount = sysconf(_SC_NPROCESSORS_ONLN);
	if (r->threads_amount == ERROR)
		r->threads_amount = THREADS_DFL;
	r->threads = malloc(sizeof(*r->threads) * r->threads_amount);
	if (!r->threads)
		fatal_error(ctx, errors(ERR_RENINIT), __FILE__, __LINE__);
	r->active = true;
	r->paused = false;
	while (r->threads_init < r->threads_amount)
	{
		if (pthread_create(&r->threads[r->threads_init], NULL, worker, ctx))
			break ;
		++r->threads_init;
	}
	return (r->threads_init == r->threads_amount);
}

static inline void	*worker(void *arg)
{
	t_context	*ctx;
	t_renderer	*r;

	ctx = (t_context *)arg;
	r = &ctx->renderer;
	while (r->active)
	{
		while (r->paused || (r->pass == 0 && r->finished))
			usleep(CPU_SPIN);
		get_render_job(ctx);
		render();
	}
	return (NULL);
}

void	render()
{
	// trace();
	// put_pixel();
}

void	restart_render_queue(t_context *ctx)
{
	(void)ctx;
}

void	get_render_job(t_context *ctx)
{
	t_renderer	*r;

	r = &ctx->renderer;
	if (r->pass != 0 && r->jobs == 0)
	{
		r->pass--;
		restart_render_queue(ctx);
	}
}
