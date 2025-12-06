#include "scene.h"
#include "camera.h"
#include "lights.h"
#include "objects.h"
#include "libft_io.h"
#include "utils.h"

static inline void	add_entity(t_context *ctx, char **params);
static inline char	**split_line(t_context *ctx, char *line);
static inline size_t	get_id_type(t_context *ctx, char *id);

void	init_scene(t_context *ctx, int fd)
{
	char	*line;
	char	**params;

	init_sky_sphere(ctx);
	try_gnl(ctx, fd, &line);
	while (line)
	{
		params = split_line(ctx, line);
		add_entity(ctx, params);
		free(line);
		try_gnl(ctx, fd, &line);
	}
}

static inline char	**split_line(t_context *ctx, char *line)
{
	char	**result;

	(void)ctx;
	(void)line;
	result = NULL;
	return (result);
}

static inline void	add_entity(t_context *ctx, char **params)
{
	static const t_add_entity	functions[] =
	{
		add_camera,
		add_light,
		add_object,
	};

	functions[get_id_type(ctx, *params)](ctx, params);
}

static inline size_t	get_id_type(t_context *ctx, char *id)
{
	static const char	*id_cam[] = {"C", NULL};
	static const char	*id_light[] = {"A", "L", NULL};
	static const char	*id_object[] = {"sp", "pl", "cy", NULL};

	if (cmp_strs(id_cam, id))
		return (0);
	else if (cmp_strs(id_light, id))
		return (1);
	else if (cmp_strs(id_object, id))
		return (2);
	else
		fatal_error(ctx, errors(ERR_EINVAL), __FILE__, __LINE__);
	return (0);
}

void	clean_scene(t_context *ctx)
{
	vector_free(&ctx->scene.objs, NULL);
	vector_free(&ctx->scene.lights, NULL);
}
