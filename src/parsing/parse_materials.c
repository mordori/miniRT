#include "libft_io.h"
#include "libft_str.h"
#include "libft_utils.h"
#include "lights.h"
#include "materials.h"
#include "objects.h"
#include "parsing.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static t_error parse_mat_fields(t_context *ctx, t_parser *p, char **tkns, int tc);

static inline bool	is_color_token(const char *str)
{
	return (str && ft_strchr(str, ',') != NULL);
}

/**
 * Parse material from either inline color or material ID reference.
 * Color format: "R,G,B" (0-255)
 * Material ID: integer index
 */
t_error	parse_material_token(t_parser *p, const char *token, t_material *out)
{
	uint32_t	id;
	t_vec3		color;
	t_material	*mat;

	if (is_color_token(token))
	{
		if (!parse_color((char *)token, &color))
			return (E_INVALID_NUM);
		*out = (t_material){0};
		out->albedo = color;
		out->base_color = BASE_COLOR;
		return (E_OK);
	}
	if (!parse_uint((char *)token, &id))
		return (E_INVALID_NUM);
	mat = get_material_by_id(p, id);
	if (!mat)
		return (E_MATERIAL);
	if (mat->is_emissive)
		return (E_EMISSIVE);
	*out = *mat;
	return (E_OK);
}

/**
 * Resolve material token to both a material struct and a material ID.
 * For mat ID tokens, returns the existing ID.
 * For inline color tokens, registers a new material and returns its ID.
 */
t_error	resolve_material(t_context *ctx, t_parser *p, const char *token,
		uint32_t *out_id)
{
	t_material	mat;
	uint32_t	id;
	t_error		err;

	err = parse_material_token(p, token, &mat);
	if (err != E_OK)
		return (err);
	if (parse_uint((char *)token, &id))
		*out_id = id;
	else
		*out_id = new_material(ctx, &mat);
	return (E_OK);
}

static t_error	parse_material_textures(t_context *ctx, char **tkn,
		t_material *mat, int token_count)
{
	t_texture	*tex;

	if (token_count >= 11)
	{
		if (is_placeholder(tkn[10]))
			return (E_OK);
		tex = find_texture_by_name(&ctx->scene, tkn[10]);
		if (!tex)
			return (E_TEXTURE);
		mat->texture = *tex;
		if (mat->texture.pixels)
			mat->base_color = BASE_TEXTURE;
	}
	if (token_count == 12)
	{
		if (is_placeholder(tkn[11]))
			return (E_OK);
		tex = find_texture_by_name(&ctx->scene, tkn[11]);
		if (!tex)
			return (E_TEXTURE);
		mat->normal_map = *tex;
		if (mat->texture.pixels)
			mat->base_color = BASE_TEXTURE;
	}
	return (E_OK);
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
	uint32_t	id;
	t_vec3		color;
	int			tc;

	tc = count_tokens(tokens);
	if (tc < 10)
		return (E_MISSING_ARGS);
	if (p->mat_count >= MAX_MATERIALS)
		return (E_TOO_MANY);
	if (!parse_uint(tokens[1], &id) || id != p->mat_count)
		return (E_MATERIAL);
	if (!parse_color(tokens[2], &color))
		return (E_INVALID_NUM);
	entry = &p->materials[p->mat_count];
	mat = &entry->material;
	*mat = (t_material){0};
	mat->albedo = color;
	mat->base_color = BASE_COLOR;
	mat->pattern_scale = 1.0f;
	return (parse_mat_fields(ctx, p, tokens, tc));
}

static t_error parse_mat_fields(t_context *ctx, t_parser *p, char **tkns, int tc)
{
	t_mat_entry *entry;
	t_material *mat;
	float emission_strength;
	t_vec3 emission_color;

	entry = &p->materials[p->mat_count];
	mat = &entry->material;
	if (!parse_float(tkns[3], &mat->metallic)
		|| !parse_float(tkns[4], &mat->roughness)
		|| !parse_float(tkns[5], &mat->ior)
		|| !parse_float(tkns[6], &mat->transmission)
		|| !parse_float(tkns[7], &emission_strength)
		|| !parse_color(tkns[8], &emission_color)
		|| !parse_uint(tkns[9], &mat->flags))
		return (E_INVALID_NUM);
	mat->emission = vec3_scale(emission_color, emission_strength);
	mat->is_emissive = (emission_strength > 0.0f);
	if (tc > 10 && parse_material_textures(ctx, tkns, mat, tc) != E_OK)
		return (E_MATERIAL);
	if (parse_mat_pattern(mat, tkns, tc) != E_OK)
		return (E_MATERIAL);
	entry->defined = true;
	new_material(ctx, mat);
	p->mat_count++;
	return (E_OK);
}
