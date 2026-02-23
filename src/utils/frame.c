#include "utils.h"

void	print_render_status(t_context *ctx, t_renderer *r)
{
	const int		bar_width = 20;
	static char		buf[128];
	static char		bar[32];
	int				hashes;
	int				i;

	hashes = (r->frame * bar_width) / r->render_samples;
	if (hashes > bar_width)
		hashes = bar_width;
	i = 0;
	while (i < bar_width)
	{
		if (i < hashes)
			bar[i] = '#';
		else
			bar[i] = ' ';
		++i;
	}
	bar[bar_width] = '\0';
	snprintf(buf, sizeof(buf), "\r\033[K\033[1;33mRendering...   [%s] [%u/%u]\033[0m", bar, r->frame, r->render_samples);
	try_write(ctx, STDOUT_FILENO, buf);
}

void	limit_polling_rate(t_renderer *r)
{
	static uint32_t		last_frame_time;

	if (r->frame < r->render_samples)
		return ;
	wait_until(last_frame_time + 17);
	last_frame_time = time_now();
}
