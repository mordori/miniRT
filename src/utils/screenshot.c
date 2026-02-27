#include "utils.h"

void	screenshot(t_context *ctx)
{
	const char		*file = "assets/screenshots/screenshot.ppm";
	int				fd;
	char			buf[64];
	t_renderer		*r;
	uint32_t		i;
	uint32_t		limit;
	uint8_t			*pixels;

	r = &ctx->renderer;
	pixels = ctx->img->pixels;
	fd = try_open(ctx, file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	snprintf(buf, sizeof(buf), "P3\n%d %d\n255\n", r->width, r->height);
	try_write(ctx, fd, buf);
	limit = r->pixels * 4;
	i = 0;
	while (i < limit)
	{
		snprintf(buf, sizeof(buf), "%d %d %d\n", pixels[i], pixels[i + 1], pixels[i + 2]);
		try_write(ctx, fd, buf);
		i += 4;
	}
	close(fd);
}
