#include "utils.h"

void	print_render_status(t_context *ctx, t_renderer *r)
{
	int					step;
	int					i;
	static const char	empty[INT64_LENGTH] = {0};
	char				str[INT64_LENGTH];

	memcpy(str, empty, INT64_LENGTH);
	try_write(ctx, STDOUT_FILENO, "\rRendering...   ");
	try_write(ctx, STDOUT_FILENO, "[");
	step = r->frame / ((RENDER_SAMPLES) / 19);
	i = step;
	while (i-- > 0)
		try_write(ctx, STDOUT_FILENO, "#");
	i = 19 - step;
	while (i-- > 0)
		try_write(ctx, STDOUT_FILENO, " ");
	try_write(ctx, STDOUT_FILENO, "] [");
	uint64_to_str(r->frame, str);
	try_write(ctx, STDOUT_FILENO, str);
	try_write(ctx, STDOUT_FILENO, "/");
	uint64_to_str(RENDER_SAMPLES, str);
	try_write(ctx, STDOUT_FILENO, str);
	try_write(ctx, STDOUT_FILENO, "] ");
}

void	limit_polling_rate(uint32_t frame)
{
	static uint32_t		last_frame_time;

	if (frame < RENDER_SAMPLES)
		return ;
	wait_until(last_frame_time + 17);
	last_frame_time = time_now();
}
