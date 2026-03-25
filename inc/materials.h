/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   materials.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:45:45 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:45:47 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIALS_H
# define MATERIALS_H

# include "defines.h"
# include "parsing.h"

t_error		new_material(t_context *ctx, t_material *mat, uint32_t *out_id);
void		set_material_data(t_path *path);
void		set_shader_data(t_path *path);

/* Texture sampling */
t_vec3		sample_texture(const t_texture *tex, t_vec2 uv);
t_vec3		get_surface_color(const t_material *mat, const t_hit *hit);
t_vec3		get_local_coords(const t_hit *hit);

/*Procedural patterns*/
t_vec3		eval_pattern(t_material *mat, t_hit *hit);
float		perlin_noise(float x, float y, float z);
float		turbulence(t_vec3 p);

t_vec3		pattern_checkerboard(const t_hit *hit, const t_material *mat);
t_vec3		pattern_gradient(const t_hit *hit, const t_material *mat);
t_vec3		pattern_stripe(const t_hit *hit, const t_material *mat);
t_vec3		pattern_spiral(const t_hit *hit, const t_material *mat);
t_vec3		pattern_grid(const t_hit *hit, const t_material *mat);
t_vec3		pattern_brick(const t_hit *hit, const t_material *mat);
t_vec3		pattern_perlin_marble(t_hit *hit, const t_material *mat);
t_vec3		pattern_perlin_wood(t_hit *hit, const t_material *mat);
t_vec3		pattern_perlin_turb(t_hit *hit, const t_material *mat);

#endif
