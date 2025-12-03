#include "scene.h"
#include "camera.h"
#include "lights.h"
#include "objects.h"
#include "libft_io.h"

t_entity	process_line(t_context *ctx, char *line)
{
	t_entity	entity;

	(void)ctx;
	(void)line;
	return (entity);
}

void	init_scene(t_context *ctx, int fd)
{
	char		*line;
	t_entity	entity;

	init_sky_sphere(ctx);
	try_gnl(ctx, fd, &line);
	while (line)
	{
		entity = process_line(ctx, line);
		validate_entity(ctx, entity);
		add_entity(ctx, entity);
		free(line);
		try_gnl(ctx, fd, &line);
	}
}

void	add_entity(t_context *ctx, t_entity entity)
{
	static const t_add_entity	functions[] =
	{
		add_camera,
		add_light,
		add_object,
		add_sky_sphere_tex
	};

	functions[entity.id](ctx, entity);
}

void	clean_scene(t_context *ctx)
{
	vector_free(&ctx->scene.objs, NULL);
	vector_free(&ctx->scene.lights, NULL);
}
