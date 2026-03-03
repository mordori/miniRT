#include "ui.h"
#include "utils.h"

static inline void	display_text_ui(t_context *ctx, char *buf, mlx_image_t **img, int *last, int current, int32_t x, int32_t y);
static inline void	display_text_f(t_context *ctx, char *buf, mlx_image_t **img, float *last, float current, int32_t x, int32_t y);
static inline void	display_status(t_context *ctx, char *buf, mlx_image_t **img, int *last, int32_t x, int32_t y);

void	update_ui(t_context *ctx)
{
	static mlx_image_t	*img_status;
	static mlx_image_t	*img_samples;
	static mlx_image_t	*img_bounces;
	static mlx_image_t	*img_f_stop;
	static mlx_image_t	*img_focal_len_mm;
	static mlx_image_t	*img_focus_dist;
	// static mlx_image_t	*img_shutter_speed;
	// static mlx_image_t	*img_iso;
	static int			last_status = 0;
	static int			last_samples = -1;
	static int			last_bounces = -1;
	static float		last_f_stop = -1.0f;
	static float		last_focal_len_mm = -1.0f;
	static float		last_focus_dist = -1.0f;
	// static float		last_shutter_speed = -1.0f;
	// static float		last_iso = -1.0f;
	static char			buf[128];


	display_status(ctx, buf, &img_status, &last_status, 50, 20);

	snprintf(buf, sizeof(buf), "Samples: %d", ctx->renderer.render_samples);
	display_text_ui(ctx, buf, &img_samples, &last_samples, ctx->renderer.render_samples, 50, 40);

	snprintf(buf, sizeof(buf), "Bounces: %d", ctx->renderer.render_bounces);
	display_text_ui(ctx, buf, &img_bounces, &last_bounces, ctx->renderer.render_bounces, 50, 60);

	snprintf(buf, sizeof(buf), "F-Stop:         f/%.1f", ctx->renderer.cam.f_stop);
	display_text_f(ctx, buf, &img_f_stop, &last_f_stop, ctx->renderer.cam.f_stop, 50, 100);
	snprintf(buf, sizeof(buf), "Focal Length:   %.0fmm", ctx->renderer.cam.focal_len_mm);
	display_text_f(ctx, buf, &img_focal_len_mm, &last_focal_len_mm, ctx->renderer.cam.focal_len_mm, 50, 120);
	snprintf(buf, sizeof(buf), "Focus Distance: %.1fm", ctx->renderer.cam.focus_dist);
	display_text_f(ctx, buf, &img_focus_dist, &last_focus_dist, ctx->renderer.cam.focus_dist, 50, 140);
// 	snprintf(buf, sizeof(buf), "ISO:            %d", (uint32_t)ctx->renderer.cam.iso);
// 	display_text_f(ctx, buf, &img_iso, &last_iso, ctx->renderer.cam.iso, 50, 120);
// 	snprintf(buf, sizeof(buf), "Shutter Speed:  %.4fs", ctx->renderer.cam.shutter_speed);
// 	display_text_f(ctx, buf, &img_shutter_speed, &last_shutter_speed, ctx->renderer.cam.shutter_speed, 50, 180);
}

void	set_text_color(mlx_image_t *img, uint32_t color)
{
	uint32_t	i;
	uint32_t	pixels;

	pixels = img->width * img->height * 4u;
	i = 0;
	while (i < pixels)
	{
		if (img->pixels[i + 3] > 0)
		{
			img->pixels[i + 0] = (color >> 24) & 0xFF;
			img->pixels[i + 1] = (color >> 16) & 0xFF;
			img->pixels[i + 2] = (color >> 8) & 0xFF;
		}
		i += 4;
	}
}

static inline void	display_status(t_context *ctx, char *buf, mlx_image_t **img, int *last, int32_t x, int32_t y)
{
	int		current;

	current = (ctx->renderer.frame >= ctx->renderer.render_samples);
	if (ctx->hide_stats)
	{
		if (*img)
		{
			mlx_delete_image(ctx->mlx, *img);
			*img = NULL;
			*last = -1;
		}
		return ;
	}
	if (*img && *last == current)
		return ;
	if (*img)
		mlx_delete_image(ctx->mlx, *img);
	if (current)
		snprintf(buf, 128, "Status:  Done!");
	else
		snprintf(buf, 128, "Status:  Rendering");
	*img = mlx_put_string(ctx->mlx, buf, x, y);
	if (!*img)
		fatal_error(ctx, errors(ERR_IMGINIT), __FILE__, __LINE__);
	if (current)
		set_text_color(*img, 0x00FF00FF);
	else
		set_text_color(*img, 0xFFFF00FF);
	*last = current;
}

static inline void	display_text_ui(t_context *ctx, char *buf, mlx_image_t **img, int *last, int current, int32_t x, int32_t y)
{
	if (ctx->hide_stats)
	{
		if (*img)
		{
			mlx_delete_image(ctx->mlx, *img);
			*img = NULL;
		}
		return ;
	}
	if (*img && *last == current)
		return ;
	if (*img)
		mlx_delete_image(ctx->mlx, *img);
	*img = mlx_put_string(ctx->mlx, buf, x, y);
	if (!*img)
		fatal_error(ctx, errors(ERR_IMGINIT), __FILE__, __LINE__);
	*last = current;
}

static inline void	display_text_f(t_context *ctx, char *buf, mlx_image_t **img, float *last, float current, int32_t x, int32_t y)
{
	if (ctx->hide_stats)
	{
		if (*img)
		{
			mlx_delete_image(ctx->mlx, *img);
			*img = NULL;
		}
		return ;
	}
	if (*img && fabsf(*last - current) < 0.00001f)
		return ;
	if (*img)
		mlx_delete_image(ctx->mlx, *img);
	*img = mlx_put_string(ctx->mlx, buf, x, y);
	if (!*img)
		fatal_error(ctx, errors(ERR_IMGINIT), __FILE__, __LINE__);
	*last = current;
}
