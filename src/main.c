#include "utils.h"
#include "camera.h"
#include "rendering.h"
#include "scene.h"

static inline void	initialize(t_context *ctx);

/**
 * @brief	Entry point to the program.
 * @mainpage
 *
 * @author		Mika Yli-Pentti		https://github.com/mordori
 * @author		Wassem Showeky		https://github.com/wshoweky
 */
int	main(int argc, char *argv[])
{
	t_context	ctx;

	if (argc != 2)
		fatal_error(NULL, errors(ERR_ARGINVL), __FILE__, __LINE__);
	ctx = (t_context){0};
	ctx.file = argv[1];
	validate_file_type(ctx.file);
	ctx.fd = try_open(ctx.file, O_RDONLY, 0);
	initialize(&ctx);
	try_write(&ctx, STDOUT_FILENO, "\033[?25h\n\nGoodbye!\n\n");
	clean(&ctx);
	return (EXIT_SUCCESS);
}

static inline void	initialize(t_context *ctx)
{
	ctx->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!ctx->mlx)
		fatal_error(ctx, errors(ERR_MLXINIT), __FILE__, __LINE__);
	ctx->img = mlx_new_image(ctx->mlx, ctx->mlx->width, ctx->mlx->height);
	if (!ctx->img || mlx_image_to_window(ctx->mlx, ctx->img, 0, 0) == ERROR)
		fatal_error(ctx, errors(ERR_IMGINIT), __FILE__, __LINE__);
	mlx_key_hook(ctx->mlx, key_hook, ctx);
	mlx_cursor_hook(ctx->mlx, cursor_hook, ctx);
	mlx_mouse_hook(ctx->mlx, mouse_hook, ctx);
	mlx_resize_hook(ctx->mlx, resize_hook, ctx);
	if (!init_renderer(ctx))
		return ;
	resize_hook(ctx->img->width, ctx->img->height, ctx);
	init_scene(ctx);
	resize_window(ctx);
	if (mlx_loop_hook(ctx->mlx, frame_loop, ctx))
		mlx_loop(ctx->mlx);
	stop_render(&ctx->renderer);
}

void	printf_init(t_context *ctx)
{
	printf("\033[?25l\nRENDER SETTINGS\n");
	printf("Samples:\t%d\n", ctx->renderer.render_samples);
	printf("Bounces:\t%d\n\n", ctx->renderer.render_bounces);

	printf("CONTROLS\n");
	printf("                 RENDER MODE                    \n");
	printf("|---------------------|------------------------|\n");
	printf("|         Key         |       Navigation       |\n");
	printf("|---------------------|------------------------|\n");
	printf("| LMB                 | Rotate                 |\n");
	printf("| W A S D             | Move                   |\n");
	printf("| L SHIFT / SPACE     | Descend / Ascend       |\n");
	printf("| n/a                 | Reset                  |\n");
	printf("|---------------------|------------------------|\n");
	printf("\n");
	printf("                  EDIT MODE                     \n");
	printf("|---------------------|------------------------|\n");
	printf("|         Key         |         Action         |\n");
	printf("|---------------------|------------------------|\n");
	printf("| TAB                 | Toggle Edit Mode       |\n");
	printf("|---------------------|------------------------|\n");
	printf("|         Key         |       Navigation       |\n");
	printf("|---------------------|------------------------|\n");
	printf("| n/a                 | Frame                  |\n");
	printf("| n/a                 | Orbit                  |\n");
	printf("| n/a                 | Zoom                   |\n");
	printf("| n/a                 | Pan                    |\n");
	printf("|---------------------|------------------------|\n");
	printf("|         Key         |        Editing         |\n");
	printf("|---------------------|------------------------|\n");
	printf("| n/a                 | Select Object          |\n");
	printf("| n/a                 | Move                   |\n");
	printf("| n/a                 | Rotate                 |\n");
	printf("| n/a                 | Scale                  |\n");
	printf("| n/a                 | Axis Constraint        |\n");
	printf("| n/a                 | Plane Constraint       |\n");
	printf("|---------------------|------------------------|\n");
	printf("\n");
	printf("                   GENERAL                      \n");
	printf("|---------------------|------------------------|\n");
	printf("|         Key         |         Action         |\n");
	printf("|---------------------|------------------------|\n");
	printf("| , / .               | Rotate Skydome         |\n");
	printf("| ↓ / ↑               | Focus Distance         |\n");
	printf("| ← / →               | F-Stop                 |\n");
	printf("| K / L               | Focal Length           |\n");
	printf("| O / P               | Samples                |\n");
	printf("| U / I               | Bounces                |\n");
	printf("| N                   | Toggle Settings        |\n");
	printf("| ESC                 | Quit                   |\n");
	printf("|---------------------|------------------------|\n");
	printf("\n");
}

void	clean(t_context *ctx)
{
	t_renderer	*r;

	if (!ctx)
		return ;
	r = &ctx->renderer;
	if (ctx->fd != ERROR)
		close(ctx->fd);
	stop_render(r);
	while (r->threads_init--)
		pthread_join(r->threads[r->threads_init], NULL);
	if (r->init_cond)
		pthread_cond_destroy(&r->cond);
	if (r->init_mutex)
		pthread_mutex_destroy(&r->mutex);
	free(r->threads);
	clean_scene(ctx);
	free_texture(&ctx->tex_bn);
	free(r->buffer);
	if (ctx->img)
		mlx_delete_image(ctx->mlx, ctx->img);
	if (ctx->mlx)
		mlx_terminate(ctx->mlx);
}
