/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_materials.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:45:57 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/27 21:12:26 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static t_error	parse_mat_fields(t_context *ctx, t_parser *p, char **tkns, int tc);
static t_error	parse_mat_values(char **tkns, t_material *mat);

/**
 * Resolve material from either inline color or material ID reference.
 * Color format: "R,G,B" (0-255) -> creates new material, returns its ID.
 * Material ID: integer index -> uses existing material ID.
 */
t_error	resolve_material(t_context *ctx, t_parser *p, const char *token, uint32_t *out_id)
{
	uint32_t	id;
	t_vec3		color;
	t_material	mat;
	t_material	*ref_mat;

	if (is_color_token(token))
	{
		if (!parse_color((char *)token, &color))
			return (E_INVALID_COLOR);
		mat = (t_material){0};
		mat.albedo = color;
		mat.base_color = BASE_COLOR;
		mat.roughness = 1.0f;
		mat.ior = 1.4f;
		return (new_material(ctx, &mat, out_id));
	}
	if (!parse_uint((char *)token, &id))
		return (E_INVALID_NUM);
	ref_mat = get_material_by_id(p, id);
	if (!ref_mat)
		return (E_MATERIAL);
	if (ref_mat->is_emissive)
		return (E_EMISSIVE);
	*out_id = id;
	return (E_OK);
}

// Shallow copy — registry owns pixel data, materials borrow it
static t_error	parse_mat_texture(t_context *ctx, char **tkn, t_material *mat, int token_count)
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
	if (token_count >= 12)
	{
		if (is_placeholder(tkn[11]))
			return (E_OK);
		tex = find_texture_by_name(&ctx->scene, tkn[11]);
		if (!tex)
			return (E_TEXTURE);
		mat->normal_map = *tex;
		if (mat->normal_map.pixels)
			mat->base_color = BASE_TEXTURE;
	}
	return (E_OK);
}

static t_error	parse_mat_values(char **tkns, t_material *mat)
{
	float	em_str;
	t_vec3	em_col;

	if (!parse_float(tkns[3], &mat->metallic)
		|| !parse_float(tkns[4], &mat->roughness)
		|| !parse_float(tkns[5], &mat->ior)
		|| !parse_float(tkns[6], &mat->transmission)
		|| !parse_float(tkns[7], &em_str)
		|| !parse_color(tkns[8], &em_col)
		|| !parse_uint(tkns[9], &mat->flags))
		return (E_INVALID_NUM);
	if (!validate_range(mat->metallic, 0.0f, 1.0f)
		|| !validate_range(mat->roughness, 0.0f, 1.0f)
		|| !validate_range(mat->ior, 1.0f, 3.0f)
		|| !validate_range(mat->transmission, 0.0f, 1.0f)
		|| !validate_range(em_str, 0.0f, 1000000.0f))
		return (E_RANGE);
	mat->emission = vec3_scale(em_col, em_str);
	mat->is_emissive = (em_str > 0.0f);
	return (E_OK);
}

static t_error	parse_mat_fields(t_context *ctx, t_parser *p, char **tkns, int tc)
{
	t_mat_entry	*entry;
	t_material	*mat;
	t_error		err;

	entry = &p->materials[p->mat_count];
	mat = &entry->material;
	err = parse_mat_values(tkns, mat);
	if (err != E_OK)
		return (err);
	if (tc > 12 && !bump_strength(mat, tkns))
		return (E_INVALID_NUM);
	if (tc > 10 && tc < 14)
		err = parse_mat_texture(ctx, tkns, mat, tc);
	else if (tc >= 14)
		err = parse_mat_pattern(mat, tkns, tc);
	if (err != E_OK)
		return (err);
	entry->defined = true;
	p->mat_count++;
	return (new_material(ctx, mat, NULL));
}

/**
 * Parse material definition line:
 *   mat <id> <color> <metallic> <roughness> <ior> <transmission>
 *       <emission_strength> <emission_color> <flags> [texture_name] [normal_map]
 *		 [bump_strength] [pattern_type] [pattern_scale] [pattern_color]
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
		return (E_ARGS);
	if (p->mat_count >= MAX_MATERIALS)
		return (E_TOO_MANY);
	if (!parse_uint(tokens[1], &id) || id != p->mat_count)
		return (E_MATERIAL);
	if (!parse_color(tokens[2], &color))
		return (E_INVALID_COLOR);
	entry = &p->materials[p->mat_count];
	mat = &entry->material;
	*mat = (t_material){0};
	mat->albedo = color;
	mat->base_color = BASE_COLOR;
	mat->pattern_scale = 1.0f;
	mat->bump_strength = 1.0f;
	return (parse_mat_fields(ctx, p, tokens, tc));
}
