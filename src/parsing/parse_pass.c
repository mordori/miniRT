/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pass.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:46:37 by wshoweky          #+#    #+#             */
/*   Updated: 2026/04/01 20:38:43 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"
#include "libft_utils.h"
#include "parsing.h"
#include "utils.h"

void try_pass(t_context* ctx, t_parser* p, char** lines, int pass) {
	int i;
	char** tokens;
	char* line;
	t_error err;

	i = -1;
	while (lines[++i]) {
		p->line_num = i + 1;
		line = lines[i];
		while (*line && ft_isspace(*line))
			line++;
		if (*line == '\0' || *line == '#')
			continue;
		tokens = try_split(line);
		if (!tokens)
			p_error(ctx, E_MALLOC, p->line_num, lines);
		err = dispatch_pass(ctx, p, tokens, pass);
		ft_free_split(tokens);
		if (err != E_OK && err != E_EMPTY)
			p_error(ctx, err, p->line_num, lines);
	}
}

t_error dispatch_pass(t_context* ctx, t_parser* p, char** tokens, int pass) {
	const char* id;

	id = tokens[0];
	if (pass == 0) {
		if (ft_strcmp(id, "tex") == 0)
			return (parse_texture_def(ctx, tokens));
		if (ft_strcmp(id, "sky") == 0)
			return (parse_skydome(ctx, tokens));
		return (E_EMPTY);
	}
	if (pass == 1) {
		if (ft_strcmp(id, "mat") == 0)
			return (parse_material_def(ctx, p, tokens));
		if (ft_strcmp(id, "render") == 0)
			return (try_render_settings(ctx, p, tokens));
		return (E_EMPTY);
	}
	if (ft_strcmp(id, "mat") == 0 || ft_strcmp(id, "tex") == 0 || ft_strcmp(id, "sky") == 0 || ft_strcmp(id, "render") == 0)
		return (E_EMPTY);
	return (identify_element(ctx, p, tokens));
}

// Render settings: render <samples> <bounces>, '_' is the placeholder
t_error try_render_settings(t_context* ctx, t_parser* p, char** tokens) {
	uint32_t val;

	if (count_tokens(tokens) != 3)
		return (E_ARGS);
	if (p->has_render_settings)
		return (E_DUPLICATE);
	if (!is_placeholder(tokens[1])) {
		if (!parse_uint(tokens[1], &val) || val == 0 || !is_pot(val))
			return (E_RANGE);
		ctx->renderer.render_samples = val;
	}
	if (!is_placeholder(tokens[2])) {
		if (!parse_uint(tokens[2], &val) || val == 0 || !is_pot(val))
			return (E_RANGE);
		ctx->renderer.render_bounces = val;
	}
	p->has_render_settings = true;
	return (E_OK);
}

void try_free_all(char** lines) {
	int i;

	i = 0;
	if (!lines)
		return;
	while (lines[i]) {
		free(lines[i]);
		lines[i] = NULL;
		i++;
	}
}
