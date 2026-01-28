#include "libft_io.h"
#include "libft_str.h"
#include "lights.h"
#include "parsing.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

/**
 * Parses material definitions with all PBR properties:
 *   mat <id> <color> <metallic> <roughness> <ior> <transmission>
 *       <emission_strength> <emission_color> <flags>
 */
t_error parse_material(t_parser *parser, char **tkns)
{
	t_mat *mat;
	t_material *material;
	int id;
	t_vec3 color;
	t_vec3 emission_color;
	float emission_strength;

	if (count_tokens(tkns) < 10)
		return (PARSE_ERR_MISSING_ARGS);
	id = ft_atof(tkns[1], NULL);
	if (id < 0 || id >= MAX_MATERIALS)
		return (PARSE_ERR_MATERIAL);
	mat = &parser->materials[id];
	material = &mat->material;
	*material = (t_material){0};
	if (!parse_color(tkns[2], &color))
		return (PARSE_ERR_INVALID_NUM);
	material->albedo = color;
	material->base_color = BASE_COLOR;
	if (!parse_float(tkns[3], &material->metallic))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_float(tkns[4], &material->roughness))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_float(tkns[5], &material->ior))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_float(tkns[6], &material->transmission))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_float(tkns[7], &emission_strength))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_color(tkns[8], &emission_color))
		return (PARSE_ERR_INVALID_NUM);
	material->emission = vec3_scale(emission_color, emission_strength);
	material->is_emissive = (emission_strength > 0.0f);
	if (!parse_float(tkns[9], &material->flags))
		return (PARSE_ERR_INVALID_NUM);
	mat->defined = true;
	parser->material_count++;
	return (PARSE_OK);
}

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
