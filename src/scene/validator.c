#include "scene.h"
#include "utils.h"
#include "libft_str.h"

void	validate_file_type(char *file)
{
	const char *dot;

	dot = ft_strchr(file, '.');
	if (!dot || ft_strcmp(dot, ".rt") != 0)
		fatal_error(NULL, "invalid file format", __FILE__, __LINE__);
}
