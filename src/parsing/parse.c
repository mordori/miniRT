/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:21 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_io.h"
#include "libft_str.h"
#include "parsing.h"
#include "utils.h"

static void read_lines(t_context* ctx, int fd, char** lines);

bool parse_scene(t_context* ctx, int fd) {
	t_parser parser;
	char* lines[MAX_LINES + 1];
	int pass;

	if (!ctx || fd < 0)
		return (false);
	parser = (t_parser){ 0 };
	ctx->renderer.render_samples = RENDER_SAMPLES;
	ctx->renderer.render_bounces = RENDER_BOUNCES;
	read_lines(ctx, fd, lines);
	pass = 0;
	while (pass < 3)
		try_pass(ctx, &parser, lines, pass++);
	validate_scene(ctx, &parser, lines);
	try_free_all(lines);
	return (true);
}

static void read_lines(t_context* ctx, int fd, char** lines) {
	char* line;
	int count;
	int status;

	count = 0;
	while (1337) {
		status = get_next_line(fd, &line);
		if (status == GNL_ERROR) {
			lines[count] = NULL;
			p_error(ctx, E_MALLOC, count + 1, lines);
		}
		if (status == GNL_EOF)
			break;
		if (count >= MAX_LINES) {
			free(line);
			lines[count] = NULL;
			p_error(ctx, E_TOO_MANY, count + 1, lines);
		}
		lines[count++] = line;
	}
	lines[count] = NULL;
}

t_error identify_element(t_context* ctx, t_parser* p, char** tokens) {
	const char* id;

	id = tokens[0];
	if (ft_strcmp(id, "A") == 0)
		return (parse_ambient(ctx, p, tokens));
	if (ft_strcmp(id, "L") == 0)
		return (parse_light(ctx, p, tokens));
	if (ft_strcmp(id, "C") == 0)
		return (parse_camera(ctx, p, tokens));
	if (ft_strcmp(id, "sp") == 0)
		return (parse_sphere(ctx, p, tokens));
	if (ft_strcmp(id, "pl") == 0)
		return (parse_plane(ctx, p, tokens));
	if (ft_strcmp(id, "cy") == 0)
		return (parse_cylinder(ctx, p, tokens));
	if (ft_strcmp(id, "co") == 0)
		return (parse_cone(ctx, p, tokens));
	if (ft_strcmp(id, "qu") == 0)
		return (parse_quad(ctx, p, tokens));
	return (E_UNKNOWN_ID);
}

void p_error(t_context* ctx, t_error err, int line_num, char** lines) {
	static char* msgs[17];

	msgs[E_UNKNOWN_ID] = "Unknown element identifier";
	msgs[E_ARGS] = "Invalid argument count for element";
	msgs[E_INVALID_NUM] = "Invalid number format";
	msgs[E_RANGE] = "Value out of valid range";
	msgs[E_DUPLICATE] = "Duplicate unique element";
	msgs[E_MALLOC] = "Memory allocation failed";
	msgs[E_MISSING_OBJ] = "Missing object in scene";
	msgs[E_TEXTURE] = "Failed to load texture";
	msgs[E_MATERIAL] = "Invalid material reference";
	msgs[E_EMISSIVE] = "Emissive material on object";
	msgs[E_TOO_MANY] = "Too many textures or materials";
	msgs[E_TOO_BIG] = "Object size is too big";
	msgs[E_NO_CAMERA] = "Missing camera (C)";
	msgs[E_INVALID_PAT] = "Invalid pattern type";
	msgs[E_INVALID_COLOR] = "Invalid color format";
	if (err > E_OK && err <= E_INVALID_COLOR) {
		try_free_all(lines);
		if (msgs[err])
			fatal_error(ctx, msgs[err], ctx->file, line_num);
		else
			fatal_error(ctx, "Unknown parsing error", ctx->file, line_num);
	}
}
