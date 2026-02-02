#include "parsing.h"
#include "libft_io.h"
#include "libft_str.h"
#include "libft_utils.h"
#include "materials.h"
#include "objects.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

t_error	parse_skydome(t_context *ctx, char **tokens)
{
	if (count_tokens(tokens) != 2)
		return (PARSE_ERR_MISSING_ARGS);
	return (load_texture_file(ctx, tokens[1], &ctx->scene.skydome));
}

void	free_tokens(char **tokens)
{
	ft_free_split(tokens);
}

void	cleanup_parser(t_parser *p)
{
	int	i;

	if (!p)
		return ;
	i = 0;
	while (i < p->tex_count)
	{
		if (p->textures[i].loaded)
			free_texture(&p->textures[i].texture);
		i++;
	}
}