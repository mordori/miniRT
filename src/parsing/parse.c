#include "libft_io.h"
#include "libft_str.h"
#include "libft_utils.h"
#include "parsing.h"
#include "utils.h"

static void				print_parse_error(t_context *ctx, t_parse_error err, int line_num);
static bool				validate_scene(t_context *ctx, t_parser *parser);
static t_parse_error	parse_line(t_context *ctx, t_parser *parser,
							char *line);
static t_parse_error	identify_input(t_context *ctx, t_parser *parser,
							char **tokens);

bool	parse_scene(t_context *ctx, int fd)
{
	t_parser		parser;
	char			*line;
	t_parse_error	err;

	if (!ctx || fd == ERROR)
		return (false);
	parser = (t_parser){0};
	while (try_gnl(ctx, fd, &line) == GNL_OK)
	{
		parser.line_num++;
		err = parse_line(ctx, &parser, line);
		free(line);
		if (err != PARSE_OK && err != PARSE_ERR_EMPTY)
		{
			print_parse_error(ctx, err, parser.line_num);
			return (false);
		}
	}
	return (validate_scene(ctx, &parser));
}

static t_parse_error	parse_line(t_context *ctx, t_parser *parser, char *line)
{
	char			**tokens;
	t_parse_error	ret;

	if (!line || !parser || !ctx)
		return (PARSE_ERR_MALLOC);
	while (*line && ft_isspace(*line))
		line++;
	if (*line == '\0' || *line == '#') // empty line or comment
		return (PARSE_ERR_EMPTY);
	tokens = try_split(line);
	if (!tokens)
		return (PARSE_ERR_MALLOC);
	ret = identify_input(ctx, parser, tokens);
	free_tokens(tokens);
	return (ret);
}

static bool	validate_scene(t_context *ctx, t_parser *parser)
{
	if (!parser)
		return (false);
	if (!parser->has_ambient)
	{
		fatal_error(ctx, "Missing ambient light (A)", __FILE__, parser->line_num);
		return (false);
	}
	if (!parser->has_light)
	{
		fatal_error(ctx, "Missing light source (L)", __FILE__, parser->line_num);
		return (false);
	}
	if (!parser->has_camera)
	{
		fatal_error(ctx, "Missing camera (C)", __FILE__, parser->line_num);
		return (false);
	}
	if (!parser->has_plane && !parser->has_sphere /* && !parser->has_cylinder */)
	{
		fatal_error(ctx, "No objects defined in the scene", __FILE__, parser->line_num);
		return (false);
	}
	return (true);
}

// key elements A, L, C must be present exactly once
static t_parse_error	identify_input(t_context *ctx, t_parser *parser,
		char **tokens)
{
	const char	*id;

	if (!tokens || !tokens[0] || !ctx || !parser)
		return (PARSE_ERR_MALLOC);
	id = tokens[0];
	if (ft_strcmp(id, "A") == 0)
		return (parse_ambient(ctx, parser, tokens));
	else if (ft_strcmp(id, "L") == 0)
		return (parse_light(ctx, parser, tokens));
	else if (ft_strcmp(id, "C") == 0)
		return (parse_camera(ctx, parser, tokens));
	else if (ft_strcmp(id, "sp") == 0)
		return (parse_sphere(ctx, parser,tokens));
	else if (ft_strcmp(id, "pl") == 0)
	    return (parse_plane(ctx, parser, tokens));
	// else if (ft_strcmp(id, "cy") == 0)
	//     return (parse_cylinder(ctx, tokens));
	else
		return (PARSE_ERR_UNKNOWN_ID);
}

static void	print_parse_error(t_context *ctx, t_parse_error err, int line_num)
{
	// ft_putstr_fd("Parse Error on line ", STDERR_FILENO);
	// ft_putnbr_fd(line_num, STDERR_FILENO);
	// ft_putstr_fd(": ", STDERR_FILENO);
	if (err == PARSE_ERR_UNKNOWN_ID)
		fatal_error(ctx, "Unknown element identifier.", 0, line_num);
	else if (err == PARSE_ERR_MISSING_ARGS)
		fatal_error(ctx, "Missing arguments for element.", 0, line_num);
	else if (err == PARSE_ERR_INVALID_NUM)
		fatal_error(ctx, "Invalid number format.", 0, line_num);
	else if (err == PARSE_ERR_RANGE)
		fatal_error(ctx, "Value out of valid range.", 0, line_num);
	else if (err == PARSE_ERR_DUPLICATE)
		fatal_error(ctx, "Duplicate unique element (A, L, or C).", 0, line_num);
	else if (err == PARSE_ERR_MALLOC)
		fatal_error(ctx, "Memory allocation failed.", 0, line_num);
	else if (err == PARSE_ERR_MISSING_OBJ)
		fatal_error(ctx, "Missing object in scene.", 0, line_num);
}
