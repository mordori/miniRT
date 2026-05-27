#include "rendering.h"
#include "utils.h"

static inline void save_render(t_context* ctx, uint8_t* pixels, uint32_t n, uint32_t i);
static inline void make_dir(t_context* ctx, const char* path);

void screenshot(t_context* ctx) {
	t_renderer* r = &ctx->renderer;

	pthread_mutex_lock(&r->mutex);
	while (r->threads_running)
		pthread_cond_wait(&r->cond, &r->mutex);
	blit(ctx, r);
	r->blit_time = time_now();
	pthread_mutex_unlock(&r->mutex);

	make_dir(ctx, "renders");
	save_render(ctx, ctx->img->pixels, ctx->renderer.pixels, 0u);
}

static inline void save_render(t_context* ctx, uint8_t* pixels, uint32_t n, uint32_t i) {
	char path[PATH_MAX];
	char buf[32];

	snprintf(path, sizeof(path), "renders/render_%s.ppm", timestamp(buf));
	int fd = try_open(ctx, path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	snprintf(buf, sizeof(buf), "P6\n%d %d\n255\n", ctx->renderer.width, ctx->renderer.height);
	try_write(ctx, fd, buf);

	uint8_t* buf_rgb = malloc(n * 3);
	if (!buf_rgb) {
		close(fd);
		fatal_error(ctx, errors(ERR_MALLOC), __FILE__, __LINE__);
	}

	while (i < n) {
		buf_rgb[i * 3 + 0] = pixels[i * 4 + 0];
		buf_rgb[i * 3 + 1] = pixels[i * 4 + 1];
		buf_rgb[i * 3 + 2] = pixels[i * 4 + 2];
		++i;
	}
	write(fd, buf_rgb, n * 3);
	free(buf_rgb);
	close(fd);
}

static inline void make_dir(t_context* ctx, const char* path) {
	if (mkdir(path, 0777) == ERROR) {
		if (errno != EEXIST)
			fatal_error(ctx, errors(ERR_DIR), __FILE__, __LINE__);
	}
}
