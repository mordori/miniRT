#include "parsing.h"
#include "utils.h"
#include "libft_str.h"
#include "libft_io.h"

// "" \t
static void print_parse_error(t_parse_error err, int line_num);
static bool validate_scene(t_parser *parser);
static t_parse_error parse_line(t_context *ctx, t_parser *parser, char *line);
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

