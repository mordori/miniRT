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
	char		*file;

	if (argc != 2)
		fatal_error(NULL, errors(ERR_ARGINVL), __FILE__, __LINE__);
	ctx = (t_context){0};
	file = argv[1];
	validate_file_type(file);
	ctx.fd = try_open(file, O_RDONLY, 0);
	initialize(&ctx);
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
	resize_hook(ctx->img->width, ctx->img->height, ctx);
	init_scene(ctx);
	resize_window(ctx);
	if (\
mlx_loop_hook(ctx->mlx, loop_hook, ctx) && \
init_renderer(ctx) && \
start_render(&ctx->renderer))
		mlx_loop(ctx->mlx);
}

void	clean(t_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->fd != ERROR)
		close(ctx->fd);
	atomic_store(&ctx->renderer.resize_pending, false);
	atomic_store(&ctx->renderer.active, false);
	while (ctx->renderer.threads_init--)
		pthread_join(ctx->renderer.threads[ctx->renderer.threads_init], NULL);
	free(ctx->renderer.threads);
	clean_scene(ctx);
	if (ctx->renderer.buffer)
		free(ctx->renderer.buffer);
	if (ctx->img)
		mlx_delete_image(ctx->mlx, ctx->img);
	if (ctx->mlx)
		mlx_terminate(ctx->mlx);
}

// viewport
// antialiasing
// dof
// --subsampling and passes with bilinear interpolation
// --ordered dithering bayer matrix / IGN
// bsdf ggx
// tonemapping
// fog
// tangent and bitangent
// shadow acne
// blue noise disk soft shadows
// scene saving
// parse .obj files
// BVH
// gamma -> srgb
// Interleaved Gradient Noise
// distance -> dim lights
// save render .png
// json file for extended objects
// bouncing ball animation, squash strect
// bilinear filterin for textures
