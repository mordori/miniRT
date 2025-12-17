#include "parsing.h"
#include "utils.h"
#include "libft_str.h"
#include "libft_io.h"

// possible parser structure for different elements
// typedef struct s_element_parser
// {
// 	const char		*id;       /* Element identifier string                 */
// 	t_parse_error	(*parse)(t_context *, char **); /* Parser function      */
// }	t_element_parser;

static void print_parse_error(t_parse_error err, int line_num);
static bool validate_scene(t_parser *parser);
static t_parse_error parse_line(t_context *ctx, t_parser *parser, char *line);
static t_parse_error identify_input(t_context *ctx, t_parser *parser, char **tokens);

// TODO:
// Parses the scene from the given file descriptor into the context.
// 1. Skips empty lines and comments.
// 2. Splits each line into tokens.
// 3. Identifies the element type and calls the appropriate parsing function.
// 4. Updates the parser state and checks for duplicates.

bool parse_scene(t_context *ctx, int fd)
{
	t_parser	parser;
	char		*line;
	t_parse_error	err;

	parser = (t_parser){0}; // Initialize all fields to zero/false
	while (try_gnl(ctx, fd, &line) == GNL_OK)
	{
		parser.line_num++;
		err = parse_line(ctx, &parser, line);
		free(line);
		if (err != PARSE_OK && err != PARSE_ERR_EMPTY)
		{
			print_parse_error(err, parser.line_num);
			return (false);
		}
	}
	return (validate_scene(&parser));
}

static t_parse_error parse_line(t_context *ctx, t_parser *parser, char *line)
{
    char **tokens;
    t_parse_error ret;

    // skip empty lines and whitespace
    while (*line && ft_isspace(*line))
        line++;
    if (*line == '\0' || *line == '#') // empty line or comment
        return (PARSE_ERR_EMPTY);
    // split line into tokens
    tokens = try_split(line);
    if (!tokens)
        return (PARSE_ERR_MALLOC);
    // identify element type and call appropriate parser
    ret = identify_input(ctx, parser, tokens);
    free_tokens(tokens);
    return (ret);
}

static bool validate_scene(t_parser *parser)
{
    if (!parser->has_ambient)
    {
        ft_putendl_fd("Error: Missing ambient light (A)", STDERR_FILENO);
        return (false);
    }
    if (!parser->has_light)
    {
        ft_putendl_fd("Error: Missing light source (L)", STDERR_FILENO);
        return (false);
    }
    if (!parser->has_camera)
    {
        ft_putendl_fd("Error: Missing camera (C)", STDERR_FILENO);
        return (false);
    }
    return (true);
}

// key elements A, L, C must be present exactly once
static t_parse_error identify_input(t_context *ctx, t_parser *parser, char **tokens)
{
    const char *id;

    if (!tokens || !tokens[0])
        return (PARSE_ERR_MALLOC);
    id = tokens[0];
    if (ft_strcmp(id, "A") == 0)
        return (parse_ambient(ctx, parser, tokens)); // TODO
    else if (ft_strcmp(id, "L") == 0)
        return (parse_light(ctx, parser, tokens)); // TODO
    else if (ft_strcmp(id, "C") == 0)
        return (parse_camera(ctx, parser, tokens)); // TODO
    // Still need to check for other elements like spheres, planes, etc..
    return (PARSE_ERR_UNKNOWN_ID);
}

static void print_parse_error(t_parse_error err, int line_num)
{
    ft_putstr_fd("Parse Error on line ", STDERR_FILENO);
    ft_putnbr_fd(line_num, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    if (err == PARSE_ERR_UNKNOWN_ID)
        ft_putendl_fd("Unknown element identifier.", STDERR_FILENO);
    else if (err == PARSE_ERR_MISSING_ARGS)
        ft_putendl_fd("Missing arguments for element.", STDERR_FILENO);
    else if (err == PARSE_ERR_INVALID_NUM)
        ft_putendl_fd("Invalid number format.", STDERR_FILENO);
    else if (err == PARSE_ERR_RANGE)
        ft_putendl_fd("Value out of valid range.", STDERR_FILENO);
    else if (err == PARSE_ERR_DUPLICATE)
        ft_putendl_fd("Duplicate unique element (A, L, or C).", STDERR_FILENO);
    else if (err == PARSE_ERR_MALLOC)
        ft_putendl_fd("Memory allocation failed.", STDERR_FILENO);
}

