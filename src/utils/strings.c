#include "utils.h"
#include "libft_str.h"

bool	cmp_strs(const char **types, const char *src)
{
	while (*types)
	{
		if (!ft_strncmp(src, *types, ft_strlen(*types)))
			return (true);
		++types;
	}
	return (false);
}
