/**
 * Parses material definitions with all PBR properties:
 *   mat <id> <color> <metallic> <roughness> <ior> <transmission>
 *       <emission_strength> <emission_color> <flags>
 */

#include "libft_io.h"
#include "libft_str.h"
#include "lights.h"
#include "parsing.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>


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
