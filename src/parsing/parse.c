#include "libft_io.h"
#include "libft_str.h"
#include "libft_utils.h"
#include "parsing.h"
#include "utils.h"

static void	read_lines(t_context *ctx, int fd, char **lines);

bool	parse_scene(t_context *ctx, int fd)
{
	t_parser	parser;
	char		*lines[MAX_LINES + 1];
	int			pass;

	if (!ctx || fd < 0)
		return (false);
	parser = (t_parser){0};
	ctx->renderer.render_samples = RENDER_SAMPLES;
	ctx->renderer.render_bounces = RENDER_BOUNCES;
	read_lines(ctx, fd, lines);
	pass = 0;
	while (pass < 3)
		try_pass(ctx, &parser, lines, pass++);
	validate_scene(ctx, &parser);
	try_free_all(lines);
	return (true);
}

static void	read_lines(t_context *ctx, int fd, char **lines)
{
	char	*line;
	int		count;

	count = 0;
	while (try_gnl(ctx, fd, &line) == GNL_OK)
	{
		if (count >= MAX_LINES)
		{
			free(line);
			lines[count] = NULL;
			try_free_all(lines);
			fatal_error(ctx, "Too many lines in scene", ctx->file, count);
		}
		lines[count++] = line;
	}
	lines[count] = NULL;
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
	if (ft_strcmp(id, "qu") == 0)
		return (parse_quad(ctx, p, tokens));
	return (E_UNKNOWN_ID);
}

void	validate_scene(t_context *ctx, t_parser *p)
{
	if (!p->has_ambient)
		fatal_error(ctx, "Missing ambient light (A)", ctx->file, p->line_num);
	if (!p->has_light)
		fatal_error(ctx, "Missing light source (L)", ctx->file, p->line_num);
	if (!p->has_camera)
		fatal_error(ctx, "Missing camera (C)", ctx->file, p->line_num);
	if (!p->has_plane && !p->has_sphere && !p->has_cylinder
		&& !p->has_cone && !p->has_quad)
		fatal_error(ctx, "No objects defined in scene", ctx->file, p->line_num);
}

void	print_error(t_context *ctx, t_error err, int line_num)
{
	static char	*msgs[13];

	msgs[E_UNKNOWN_ID] = "Unknown element identifier";
	msgs[E_ARGS] = "Invalid argument count for element";
	msgs[E_INVALID_NUM] = "Invalid number format";
	msgs[E_RANGE] = "Value out of valid range";
	msgs[E_DUPLICATE] = "Duplicate unique element (A, L, or C)";
	msgs[E_MALLOC] = "Memory allocation failed";
	msgs[E_MISSING_OBJ] = "Missing object in scene";
	msgs[E_TEXTURE] = "Failed to load texture";
	msgs[E_MATERIAL] = "Invalid material reference";
	msgs[E_EMISSIVE] = "Emissive material on object / "
		"non-emissive material on light";
	msgs[E_TOO_MANY] = "Too many textures or materials";
	if (err > E_OK && err <= E_TOO_MANY)
		fatal_error(ctx, msgs[err], ctx->file, line_num);
}
