#include "rendering.h"
#include "utils.h"

static inline void save_render(t_context* ctx, char* out_filepath, size_t max_len);

void screenshot(t_context* ctx, char* out_filepath, size_t max_len) {
	t_renderer* r = &ctx->renderer;

	pthread_mutex_lock(&ctx->renderer.mutex);
	while (r->threads_running)
		pthread_cond_wait(&r->cond, &r->mutex);
	bool is_final = r->frame >= r->render_samples && r->mode == RENDERED;
	blit(ctx, r, is_final);
	r->blit_time = time_now();

	make_dir(ctx, "renders");
	save_render(ctx, out_filepath, max_len);
	pthread_mutex_unlock(&ctx->renderer.mutex);
}

static inline void save_render(t_context* ctx, char* out_filepath, size_t max_len) {
	char buf[32];

	snprintf(out_filepath, max_len, "renders/render_%s.ppm", timestamp(buf));
	int fd = try_open(ctx, out_filepath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	snprintf(buf, sizeof(buf), "P6\n%d %d\n255\n", ctx->renderer.width, ctx->renderer.height);
	try_write(ctx, fd, buf);

	uint32_t n = ctx->renderer.pixels;
	uint8_t* buf_rgb = malloc(n * 3);
	if (!buf_rgb) {
		close(fd);
		pthread_mutex_unlock(&ctx->renderer.mutex);
		fatal_error(ctx, errors(ERR_MALLOC), __FILE__, __LINE__);
	}

	uint8_t* pixels = ctx->img->pixels;
	uint32_t i = 0;
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
