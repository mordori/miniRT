#include "libft_utils.h"
#include "parsing.h"
#include "utils.h"

void	try_pass(t_context *ctx, t_parser *p,
				char **lines, int pass)
{
	int		i;
	char	**tokens;
	char	*line;
	t_error	err;

	i = -1;
	while (lines[++i])
	{
		p->line_num = i + 1;
		line = lines[i];
		while (*line && ft_isspace(*line))
			line++;
		if (*line == '\0' || *line == '#')
			continue ;
		tokens = try_split(line);
		if (!tokens)
			print_error(ctx, E_MALLOC, p->line_num);
		err = dispatch_pass(ctx, p, tokens, pass);
		free_tokens(tokens);
		if (err != E_OK && err != E_EMPTY)
			print_error(ctx, err, p->line_num);
	}
}

// Render settings: render <samples> <bounces>, '_' is the placeholder
t_error	try_render_settings(t_context *ctx, char **tokens)
{
	uint32_t	val;

	if (count_tokens(tokens) != 3)
		return (E_MISSING_ARGS);
	if (!is_placeholder(tokens[1]))
	{
		if (!parse_uint(tokens[1], &val) || val == 0)
			return (E_RANGE);
		ctx->renderer.render_samples = val;
	}
	if (!is_placeholder(tokens[2]))
	{
		if (!parse_uint(tokens[2], &val) || val == 0)
			return (E_RANGE);
		ctx->renderer.refine_bounces = val;
	}
	return (E_OK);
}

void	try_free_all(char **lines, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(lines[i++]);
}
