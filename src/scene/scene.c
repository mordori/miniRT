#include "scene.h"
#include "camera.h"
#include "lights.h"
#include "objects.h"
#include "parsing.h"
#include "libft_io.h"
#include "utils.h"

// static inline void	add_entity(t_context *ctx, char **params);
// static inline char	**split_line(t_context *ctx, char *line);
// static inline int	get_ent_type(t_context *ctx, char *param);

void	init_scene(t_context *ctx)
{
	vector_try_init(ctx, &ctx->scene.objs, false, free);
	vector_try_init(ctx, &ctx->scene.lights, false, free);
	init_skydome(ctx, "assets/textures/sky.png"); //TODO: to be add into .rt and parsed with ambient light
	if (!parse_scene(ctx, ctx->fd))
		fatal_error(ctx, "Failed to parse scene file", __FILE__, __LINE__);
	// For testing rendering
	// -----------------------
	// 	char *line;
	// 	char **params = NULL;
	// 	(void)line;
	// 	add_camera(ctx, params);
	// 	add_light(ctx, params);
	// 	add_object(ctx, params);
	// 	ctx->scene.ambient_light.color = (t_vec4){{1.0f, 1.0f, 1.0f, 1.0f}};
	// 	ctx->scene.ambient_light.intensity = 0.12f;
	// -----------------------
	// try_gnl(ctx, ctx->fd, &line);
	// while (line)
	// {
	// 	params = split_line(ctx, line);
	// 	add_entity(ctx, params);
	// 	free(line);
	// 	try_gnl(ctx, ctx->fd, &line);
	// }
	close(ctx->fd);
	ctx->fd = ERROR;
	init_bvh(ctx);
}

// static inline void	add_entity(t_context *ctx, char **params)
// {
// 	static const t_add_entity	functions[] =
// 	{
// 		new_camera,
// 		new_light,
// 		new_object
// 	};

// 	if (!params || !*params)
// 		return ;
// 	functions[get_ent_type(ctx, *params)](ctx, params);
// }

// static inline int	get_ent_type(t_context *ctx, char *param)
// {
// 	static const char	*id_cam[] = {"C", NULL};
// 	static const char	*id_light[] = {"A", "L", NULL};
// 	static const char	*id_object[] = {"sp", "pl", "cy", NULL};

// 	if (cmp_strs(id_cam, param))
// 		return (ENT_CAMERA);
// 	if (cmp_strs(id_light, param))
// 		return (ENT_LIGHT);
// 	if (cmp_strs(id_object, param))
// 		return (ENT_OBJECT);
// 	fatal_error(ctx, errors(ERR_EINVAL), __FILE__, __LINE__);
// 	return (ERROR);
// }

void	clean_scene(t_context *ctx)
{
	clean_bvh(ctx->scene.bvh_root);
	vector_free(&ctx->scene.objs, &ctx->scene.lights, NULL);
	free_texture(&ctx->scene.skydome);
}
