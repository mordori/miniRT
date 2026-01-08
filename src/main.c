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
	if (!init_renderer(ctx))
		return ;
	resize_hook(ctx->img->width, ctx->img->height, ctx);
	init_scene(ctx);
	resize_window(ctx);
	if (mlx_loop_hook(ctx->mlx, loop_hook, ctx) && start_render(&ctx->renderer))
		mlx_loop(ctx->mlx);
}

void	clean(t_context *ctx)
{
	t_renderer	*r;

	if (!ctx)
		return ;
	r = &ctx->renderer;
	if (ctx->fd != ERROR)
		close(ctx->fd);
	pthread_mutex_lock(&r->mutex);
		r->active = false;
		r->resize_pending = false;
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);
	while (r->threads_init--)
		pthread_join(r->threads[r->threads_init], NULL);
	if (r->init_cond)
		pthread_cond_destroy(&r->cond);
	if (r->init_mutex)
		pthread_mutex_destroy(&r->mutex);
	free(r->threads);
	clean_scene(ctx);
	if (r->buffer)
		free(r->buffer);
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
