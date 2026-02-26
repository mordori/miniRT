#include "libft_utils.h"
#include "libft_str.h"
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
		ft_free_split(tokens);
		if (err != E_OK && err != E_EMPTY)
			print_error(ctx, err, p->line_num);
	}
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

// Render settings: render <samples> <bounces>, '_' is the placeholder
t_error	try_render_settings(t_context *ctx, char **tokens)
{
	uint32_t	val;

	if (count_tokens(tokens) != 3)
		return (E_ARGS);
	if (!is_placeholder(tokens[1]))
	{
		if (!parse_uint(tokens[1], &val) || val == 0 || !ft_is_pot(val))
			return (E_RANGE);
		ctx->renderer.render_samples = val;
	}
	if (!is_placeholder(tokens[2]))
	{
		if (!parse_uint(tokens[2], &val) || val == 0 || !ft_is_pot(val))
			return (E_RANGE);
		ctx->renderer.render_bounces = val;
	}
	return (E_OK);
}

void	try_free_all(char **lines)
{
	int	i;

	i = 0;
	if (!lines)
		return ;
	while (lines[i])
	{
		free(lines[i]);
		lines[i] = NULL;
		i++;
	}
}
