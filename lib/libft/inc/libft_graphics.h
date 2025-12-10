/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_graphics.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 20:12:40 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/10 02:53:45 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_GRAPHICS_H
# define LIBFT_GRAPHICS_H

# include "libft_defs.h"
# include "libft_math.h"

struct s_vertex
{
	t_vec4		raw_pos;
	t_vec4		pos;
	t_vec2i		screen_pos;
	uint32_t	color;
	float		depth;
};

struct s_transform
{
	t_vec3	pos;
	t_vec3	rot;
	t_vec3	scale;
};

struct s_ray
{
	t_vec3	origin;
	t_vec3	dir;
	t_vec3	inv_dir;
	float	t;
	int		sign[3];
};


struct s_quad
{
	float	top_left;
	float	top_right;
	float	bottom_left;
	float	bottom_right;
};

#endif
