#include "libft_io.h"
#include "libft_str.h"
#include "libft_utils.h"
#include "lights.h"
#include "parsing.h"
#include "materials.h"
#include "objects.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static inline bool	is_color_token(const char *str)
{
	return (str && ft_strchr(str, ',') != NULL);
}

t_material	*get_material_by_id(t_parser *p, int id)
{
	if (id < 0 || id >= p->mat_count || !p->materials[id].defined)
		return (NULL);
	return (&p->materials[id].material);
}

/**
 * Parse material from either inline color or material ID reference.
 * Color format: "R,G,B" (0-255)
 * Material ID: integer index
 */
t_error	parse_material_token(t_parser *p, const char *token, t_material *out)
{
	int			id;
	t_vec3		color;
	t_material	*mat;

	if (is_color_token(token))
	{
		if (!parse_color((char *)token, &color))
			return (PARSE_ERR_INVALID_NUM);
		*out = (t_material){0};
		out->albedo = color;
		out->base_color = BASE_COLOR;
		return (PARSE_OK);
	}
	if (!parse_int((char *)token, &id))
		return (PARSE_ERR_INVALID_NUM);
	mat = get_material_by_id(p, id);
	if (!mat)
		return (PARSE_ERR_MATERIAL);
	*out = *mat;
	return (PARSE_OK);
}

static t_error parse_material_texture(t_parser *p, t_material *mat, const char *tkn)
{
	t_texture	*tex;

	if (is_placeholder(tkn))
		return (PARSE_OK);
	tex = find_texture_by_name(p, tkn);
	if (!tex)
		return (PARSE_ERR_TEXTURE);
	mat->texture = *tex;
	if (mat->texture.pixels)
		mat->base_color = BASE_TEXTURE;
	return (PARSE_OK);
}

/**
 * Parse material definition line:
 *   mat <id> <color> <metallic> <roughness> <ior> <transmission>
 *       <emission_strength> <emission_color> <flags>
 */
t_error	parse_material_def(t_context *ctx, t_parser *p, char **tokens)
{
	t_mat_entry	*entry;
	t_material	*mat;
	int			id;
	t_vec3		color;
	t_vec3		emission_color;
	float		emission_strength;
	int 		tokens_count;

	tokens_count = count_tokens(tokens);
	if (tokens_count < 10)
		return (PARSE_ERR_MISSING_ARGS);
	if (p->mat_count >= MAX_MATERIALS)
		return (PARSE_ERR_TOO_MANY);
	if (!parse_int(tokens[1], &id) || id != p->mat_count)
		return (PARSE_ERR_MATERIAL);
	if (!parse_color(tokens[2], &color))
		return (PARSE_ERR_INVALID_NUM);
	entry = &p->materials[p->mat_count];
	mat = &entry->material;
	*mat = (t_material){0};
	mat->albedo = color;
	mat->base_color = BASE_COLOR;
	if (!parse_float(tokens[3], &mat->metallic)
		|| !parse_float(tokens[4], &mat->roughness)
		|| !parse_float(tokens[5], &mat->ior)
		|| !parse_float(tokens[6], &mat->transmission)
		|| !parse_float(tokens[7], &emission_strength)
		|| !parse_color(tokens[8], &emission_color)
		|| !parse_uint(tokens[9], &mat->flags))
		return (PARSE_ERR_INVALID_NUM);
	mat->emission = vec3_scale(emission_color, emission_strength);
	mat->is_emissive = (emission_strength > 0.0f);
	if (tokens_count > 10 && parse_material_texture(p, mat, tokens[10]) != PARSE_OK)
		return (PARSE_ERR_MATERIAL);
	entry->defined = true;
	new_material(ctx, mat);
	p->mat_count++;
	return (PARSE_OK);
}
