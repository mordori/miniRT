/**
 * Material and Texture Parsing
 *
 * Parses material definitions with all PBR properties:
 *   mat <id> <color> <metallic> <roughness> <ior> <transmission>
 *       <emission_strength> <emission_color> <flags>
 *
 * Emission vec3 = emission_color * emission_strength (normalized)
 */

#include "libft_io.h"
#include "libft_str.h"
#include "lights.h"
#include "parsing.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

/**
 * Check if a string ends with .png
 */
bool	is_png_filename(const char *str)
{
	size_t	len;

	if (!str)
		return (false);
	len = ft_strlen(str);
	if (len < 5)
		return (false);
	return (ft_strcmp(str + len - 4, ".png") == 0);
}

/**
 * Cleanup all loaded textures in the parser.
 */
void	cleanup_parser(t_parser *p)
{
	int	i;

	if (!p)
		return ;
	i = 0;
	while (i < p->texture_count)
	{
		if (p->textures[i].loaded)
			free_texture(&p->textures[i].texture);
		i++;
	}
}
