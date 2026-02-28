#include "utils.h"

void	make_dir(t_context *ctx, const char *path)
{
	if (mkdir(path, 0777) == ERROR)
		if (errno != EEXIST)
			fatal_error(ctx, errors(ERR_DIR), __FILE__, __LINE__);
}

char	*timestamp(char *ts)
{
	time_t		now;

	now = time(NULL);
	strftime(ts, 32, "%Y-%m-%d_%H-%M-%S", localtime(&now));
	return (ts);
}

void	screenshot(t_context *ctx, uint8_t *pixels, uint32_t n)
{
	char			path[PATH_MAX];
	char			ts[32];
	char			buf[32];
	int				fd;
	uint32_t		i;
	uint8_t			*buffer_rgb;

	make_dir(ctx, "renders");
	snprintf(path, sizeof(path), "renders/render_%s.ppm", timestamp(ts));
	fd = try_open(ctx, path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	snprintf(buf, sizeof(buf), "P6\n%d %d\n255\n", ctx->renderer.width, ctx->renderer.height);
	try_write(ctx, fd, buf);
	buffer_rgb = malloc(n * 3);
	i = 0;
	while (i < n)
	{
		buffer_rgb[i * 3 + 0] = pixels[i * 4 + 0];
		buffer_rgb[i * 3 + 1] = pixels[i * 4 + 1];
		buffer_rgb[i * 3 + 2] = pixels[i * 4 + 2];
		++i;
	}
	write(fd, buffer_rgb, n * 3);
	free(buffer_rgb);
	close(fd);
}
