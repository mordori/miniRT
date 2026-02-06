#include "libft_io.h"
#include "libft_str.h"
#include "libft_utils.h"
#include "parsing.h"
#include "utils.h"

static void		print_error(t_context *ctx, t_error err, int line_num);
static bool		validate_scene(t_context *ctx, t_parser *p);
static t_error	parse_line(t_context *ctx, t_parser *p, char *line);
static t_error	identify_input(t_context *ctx, t_parser *p, char **tokens);

bool	parse_scene(t_context *ctx, int fd)
{
	t_parser	parser;
	char		*line;
	t_error		err;

	if (!ctx || fd < 0)
		return (false);
	parser = (t_parser){0};
	while (try_gnl(ctx, fd, &line) == GNL_OK)
	{
		parser.line_num++;
		err = parse_line(ctx, &parser, line);
		free(line);
		if (err != PARSE_OK && err != PARSE_ERR_EMPTY)
		{
			print_error(ctx, err, parser.line_num);
			cleanup_parser(&parser);
			return (false);
		}
	}
	if (!validate_scene(ctx, &parser))
	{
		cleanup_parser(&parser);
		return (false);
	}
	return (true);
}

static t_error	parse_line(t_context *ctx, t_parser *p, char *line)
{
	char	**tokens;
	t_error	ret;

	while (*line && ft_isspace(*line))
		line++;
	if (*line == '\0' || *line == '#')
		return (PARSE_ERR_EMPTY);
	tokens = try_split(line);
	if (!tokens)
		return (PARSE_ERR_MALLOC);
	ret = identify_input(ctx, p, tokens);
	free_tokens(tokens);
	return (ret);
}

static bool	validate_scene(t_context *ctx, t_parser *p)
{
	if (!p->has_ambient)
	{
		fatal_error(ctx, "Missing ambient light (A)", __FILE__, p->line_num);
		return (false);
	}
	if (!p->has_light)
	{
		fatal_error(ctx, "Missing light source (L)", __FILE__, p->line_num);
		return (false);
	}
	if (!p->has_camera)
	{
		fatal_error(ctx, "Missing camera (C)", __FILE__, p->line_num);
		return (false);
	}
	if (!p->has_plane && !p->has_sphere && !p->has_cylinder)
	{
		fatal_error(ctx, "No objects defined in scene", __FILE__, p->line_num);
		return (false);
	}
	return (true);
}

static t_error	identify_input(t_context *ctx, t_parser *p, char **tokens)
{
	const char	*id;

	if (!tokens[0])
		return (PARSE_ERR_MALLOC);
	id = tokens[0];
	if (ft_strcmp(id, "mat") == 0)
		return (parse_material_def(ctx, p, tokens));
	if (ft_strcmp(id, "tex") == 0)
		return (parse_texture_def(ctx, p, tokens));
	if (ft_strcmp(id, "A") == 0)
		return (parse_ambient(ctx, p, tokens));
	if (ft_strcmp(id, "L") == 0)
		return (parse_light(ctx, p, tokens));
	if (ft_strcmp(id, "C") == 0)
		return (parse_camera(ctx, p, tokens));
	if (ft_strcmp(id, "sky") == 0)
		return (parse_skydome(ctx, tokens));
	if (ft_strcmp(id, "sp") == 0)
		return (parse_sphere(ctx, p, tokens));
	if (ft_strcmp(id, "pl") == 0)
		return (parse_plane(ctx, p, tokens));
	if (ft_strcmp(id, "cy") == 0)
		return (parse_cylinder(ctx, p, tokens));
	return (PARSE_ERR_UNKNOWN_ID);
}

static void	print_error(t_context *ctx, t_error err, int line_num)
{
	static char	*msgs[] = {
	[PARSE_ERR_UNKNOWN_ID] = "Unknown element identifier",
	[PARSE_ERR_MISSING_ARGS] = "Missing arguments for element",
	[PARSE_ERR_INVALID_NUM] = "Invalid number format",
	[PARSE_ERR_RANGE] = "Value out of valid range",
	[PARSE_ERR_DUPLICATE] = "Duplicate unique element (A, L, or C)",
	[PARSE_ERR_MALLOC] = "Memory allocation failed",
	[PARSE_ERR_MISSING_OBJ] = "Missing object in scene",
	[PARSE_ERR_TEXTURE] = "Failed to load texture",
	[PARSE_ERR_MATERIAL] = "Invalid material reference",
	[PARSE_ERR_TOO_MANY] = "Too many textures or materials"};

	if (err > PARSE_OK && err <= PARSE_ERR_TOO_MANY)
		fatal_error(ctx, msgs[err], NULL, line_num);
}
