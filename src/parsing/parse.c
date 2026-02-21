#include "libft_io.h"
#include "libft_str.h"
#include "libft_utils.h"
#include "parsing.h"
#include "utils.h"

bool	parse_scene(t_context *ctx, int fd)
{
	t_parser	parser;
	char		*line;
	char		*lines[MAX_LINES + 1];
	int			count;

	if (!ctx || fd < 0)
		return (false);
	parser = (t_parser){0};
	count = 0;
	ctx->renderer.render_samples = RENDERED_SAMPLES;
	ctx->renderer.rendered_bounces = RENDERED_BOUNCES;
	while (try_gnl(ctx, fd, &line) == GNL_OK)
	{
		if (count >= MAX_LINES)
		{
			free(line);
			try_free_all(lines, count);
			fatal_error(ctx, "Too many lines in scene", __FILE__, count);
		}
		lines[count++] = line;
	}
	lines[count] = NULL;
	count = 0;
	while (count < 3)
		try_pass(ctx, &parser, lines, count++);
	validate_scene(ctx, &parser);
	try_free_all(lines, count);
	return (true);
}

t_error	dispatch_pass(t_context *ctx, t_parser *p,
					char **tokens, int pass)
{
	const char	*id;

	id = tokens[0];
	if (pass == 0)
	{
		if (ft_strcmp(id, "tex") == 0)
			return (parse_texture_def(ctx, tokens));
		if (ft_strcmp(id, "sky") == 0)
			return (parse_skydome(ctx, tokens));
		return (E_EMPTY);
	}
	if (pass == 1)
	{
		if (ft_strcmp(id, "mat") == 0)
			return (parse_material_def(ctx, p, tokens));
		if (ft_strcmp(id, "render") == 0)
			return (try_render_settings(ctx, tokens));
		return (E_EMPTY);
	}
	if (ft_strcmp(id, "mat") == 0 || ft_strcmp(id, "tex") == 0
		|| ft_strcmp(id, "sky") == 0 || ft_strcmp(id, "render") == 0)
		return (E_EMPTY);
	return (identify_element(ctx, p, tokens));
}

t_error	identify_element(t_context *ctx, t_parser *p, char **tokens)
{
	const char	*id;

	id = tokens[0];
	if (ft_strcmp(id, "A") == 0)
		return (parse_ambient(ctx, p, tokens));
	if (ft_strcmp(id, "L") == 0)
		return (parse_light(ctx, p, tokens));
	if (ft_strcmp(id, "C") == 0)
		return (parse_camera(ctx, p, tokens));
	if (ft_strcmp(id, "sp") == 0)
		return (parse_sphere(ctx, p, tokens));
	if (ft_strcmp(id, "pl") == 0)
		return (parse_plane(ctx, p, tokens));
	if (ft_strcmp(id, "cy") == 0)
		return (parse_cylinder(ctx, p, tokens));
	if (ft_strcmp(id, "co") == 0)
		return (parse_cone(ctx, p, tokens));
	return (E_UNKNOWN_ID);
}

void	validate_scene(t_context *ctx, t_parser *p)
{
	if (!p->has_ambient)
		fatal_error(ctx, "Missing ambient light (A)", __FILE__, p->line_num);
	if (!p->has_light)
		fatal_error(ctx, "Missing light source (L)", __FILE__, p->line_num);
	if (!p->has_camera)
		fatal_error(ctx, "Missing camera (C)", __FILE__, p->line_num);
	if (!p->has_plane && !p->has_sphere && !p->has_cylinder && !p->has_cone)
		fatal_error(ctx, "No objects defined in scene", __FILE__, p->line_num);
}

void	print_error(t_context *ctx, t_error err, int line_num)
{
	static char	*msgs[] = {
	[E_UNKNOWN_ID] = "Unknown element identifier",
	[E_MISSING_ARGS] = "Missing arguments for element",
	[E_INVALID_NUM] = "Invalid number format",
	[E_RANGE] = "Value out of valid range",
	[E_DUPLICATE] = "Duplicate unique element (A, L, or C)",
	[E_MALLOC] = "Memory allocation failed",
	[E_MISSING_OBJ] = "Missing object in scene",
	[E_TEXTURE] = "Failed to load texture",
	[E_MATERIAL] = "Invalid material reference",
	[E_EMISSIVE] = "Emissive material on object / "
		"non-emissive material on light",
	[E_TOO_MANY] = "Too many textures or materials"};

	if (err > E_OK && err <= E_TOO_MANY)
		fatal_error(ctx, msgs[err], NULL, line_num);
}
