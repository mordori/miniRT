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
		return (E_MISSING_ARGS);
	return (load_texture_file(tokens[1], &ctx->scene.skydome));
}

inline bool	is_placeholder(const char *str)
{
	return (str && ft_strcmp(str, "_") == 0);
}

t_material	*get_material_by_id(t_parser *p, uint32_t id)
{
	if (id >= p->mat_count || !p->materials[id].defined)
		return (NULL);
	return (&p->materials[id].material);
}

int	count_tokens(char **tokens)
{
	int	count;

	if (!tokens)
		return (0);
	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

int	count_delimiter(const char *str, char delim)
{
	int	count;

	if (!str)
		return (0);
	count = 0;
	while (*str)
	{
		if (*str == delim)
			count++;
		str++;
	}
	return (count);
}

void	free_tokens(char **tokens)
{
	ft_free_split(tokens);
}
