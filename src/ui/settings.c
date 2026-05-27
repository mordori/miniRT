#include "ui.h"
#include "utils.h"

static inline void display_text_uint(t_context* ctx, mlx_image_t** img, t_ui vals, t_vec2i pos);
static inline void display_text_f(t_context* ctx, mlx_image_t** img, t_ui vals, t_vec2i pos);
static inline void display_status(t_context* ctx, int* last, t_vec2i pos, int current);
static inline bool img_handling(t_context* ctx, mlx_image_t** img, int* last, int current);

static char g_buf[128];

void update_ui(t_context* ctx) {
	static mlx_image_t* img[5];
	static int last_i[3] = { 0, -1, -1 };
	static float last_f[3] = { -1.0f, -1.0f, -1.0f };

	display_status(ctx, &last_i[0], vec2i(50, 20), 0);

	snprintf(g_buf, sizeof(g_buf), "Samples: %d", ctx->renderer.render_samples);
	display_text_uint(ctx, &img[0], ui_val(&last_i[1], ctx->renderer.render_samples), vec2i(50, 40));

	snprintf(g_buf, sizeof(g_buf), "Bounces: %d", ctx->renderer.render_bounces);
	display_text_uint(ctx, &img[1], ui_val(&last_i[2], ctx->renderer.render_bounces), vec2i(50, 60));

	snprintf(g_buf, sizeof(g_buf), "F-Stop:         f/%.1f", ctx->renderer.cam.f_stop);
	display_text_f(ctx, &img[2], ui_valf(&last_f[0], ctx->renderer.cam.f_stop), vec2i(50, 100));

	snprintf(g_buf, sizeof(g_buf), "Focal Length:   %.0fmm", ctx->renderer.cam.focal_len_mm);
	display_text_f(ctx, &img[3], ui_valf(&last_f[1], ctx->renderer.cam.focal_len_mm), vec2i(50, 120));

	snprintf(g_buf, sizeof(g_buf), "Focus Distance: %.1fm", ctx->renderer.cam.focus_dist);
	display_text_f(ctx, &img[4], ui_valf(&last_f[2], ctx->renderer.cam.focus_dist), vec2i(50, 140));
}

static inline void display_status(t_context* ctx, int* last, t_vec2i pos, int current) {
	static const char* edit[] = { "Selected", "Translate", "Rotate", "Scale" };
	static const char* axis[] = { "", "X", "Y", "Z", "XY", "XZ", "YZ" };
	static const char* mode[] = { "Rendering", "Done!", "Editing", "Selected" };
	static mlx_image_t* img;

	current = (ctx->renderer.frame >= ctx->renderer.render_samples);
	if (ctx->renderer.mode == SOLID)
		current = 2 + (ctx->editor.selected_obj != NULL) + ctx->editor.mode * 10 + ctx->editor.constraint_axis * 100;
	if (!img_handling(ctx, &img, last, current))
		return;

	if (ctx->renderer.mode != SOLID)
		snprintf(g_buf, 128, "Status:  %s", mode[current]);
	else if (ctx->editor.mode == EDIT_DEFAULT)
		snprintf(g_buf, 128, "Status:  %s", mode[2 + (ctx->editor.selected_obj != NULL)]);
	else
		snprintf(g_buf, 128, "Status:  %s %s", edit[ctx->editor.mode], axis[ctx->editor.constraint_axis]);

	img = mlx_put_string(ctx->mlx, g_buf, pos.x, pos.y);
	if (!img)
		fatal_error(ctx, errors(ERR_IMGINIT), __FILE__, __LINE__);
	set_text_color(img, 0xFFFF00FF);
	if (ctx->renderer.mode == SOLID)
		set_text_color(img, 0x00FFFFFF);
	else if (current)
		set_text_color(img, 0x88FF00FF);
	*last = current;
}

static inline void display_text_uint(t_context* ctx, mlx_image_t** img, t_ui vals, t_vec2i pos) {
	if (ctx->hide_stats) {
		if (*img) {
			mlx_delete_image(ctx->mlx, *img);
			*img = NULL;
		}
		return;
	}

	if (*img && *vals.last == vals.current)
		return;

	if (*img)
		mlx_delete_image(ctx->mlx, *img);
	*img = mlx_put_string(ctx->mlx, g_buf, pos.x, pos.y);
	if (!*img)
		fatal_error(ctx, errors(ERR_IMGINIT), __FILE__, __LINE__);
	*vals.last = vals.current;
}

static inline void display_text_f(t_context* ctx, mlx_image_t** img, t_ui vals, t_vec2i pos) {
	if (ctx->hide_stats) {
		if (*img) {
			mlx_delete_image(ctx->mlx, *img);
			*img = NULL;
		}
		return;
	}

	if (*img && fabsf(*vals.lastf - vals.currentf) < 0.00001f)
		return;

	if (*img)
		mlx_delete_image(ctx->mlx, *img);
	*img = mlx_put_string(ctx->mlx, g_buf, pos.x, pos.y);
	if (!*img)
		fatal_error(ctx, errors(ERR_IMGINIT), __FILE__, __LINE__);
	*vals.lastf = vals.currentf;
}

static inline bool img_handling(t_context* ctx, mlx_image_t** img, int* last, int current) {
	if (ctx->hide_stats) {
		if (*img) {
			mlx_delete_image(ctx->mlx, *img);
			*img = NULL;
			*last = -1;
		}
		return false;
	}

	if (*img && *last == current)
		return false;
	if (*img)
		mlx_delete_image(ctx->mlx, *img);
	return true;
}
