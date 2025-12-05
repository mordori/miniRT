#include "utils.h"
#include "camera.h"
#include "rendering.h"
#include "scene.h"

static inline void	setup_mlx(t_context *ctx);

int	main(int argc, char *argv[])
{
	t_context	ctx;
	char		*file;
	int			fd;

	if (argc != 2)
		fatal_error(NULL, "invalid number of arguments");
	ctx = (t_context){0};
	file = argv[1];
	validate_file_type(file);
	fd = try_open(file, O_RDONLY, 0);
	init_scene(&ctx, fd);
	setup_mlx(&ctx);
	clean(&ctx);
	return (EXIT_SUCCESS);
}

static inline void	setup_mlx(t_context *ctx)
{
	mlx_set_setting(MLX_MAXIMIZED, true);
	ctx->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!ctx->mlx)
		fatal_error(ctx, "mlx init failed");
	ctx->img = mlx_new_image(ctx->mlx, ctx->mlx->width, ctx->mlx->height);
	if (!ctx->img || mlx_image_to_window(ctx->mlx, ctx->img, 0, 0) == ERROR)
		fatal_error(ctx, "mlx img failed");
	mlx_key_hook(ctx->mlx, key_hook, ctx);
	mlx_resize_hook(ctx->mlx, resize_hook, ctx);
	if (mlx_loop_hook(ctx->mlx, update_hook, ctx) && init_renderer(ctx))
		mlx_loop(ctx->mlx);
}

void	clean(t_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->img)
		mlx_delete_image(ctx->mlx, ctx->img);
	if (ctx->mlx)
		mlx_terminate(ctx->mlx);
	while (ctx->renderer.threads_init--)
		pthread_join(ctx->renderer.threads[ctx->renderer.threads_init], NULL);
	free(ctx->renderer.threads);
	clean_scene(ctx);
}

// viewport
// antialiasing
// dof
// --subsampling and passes with bilinear interpolation
// --ordered dithering, bayer matrix
// bsdf material
// tonemapping
// fog
// tangent and bitangent
// shadow acne
// scene saving
