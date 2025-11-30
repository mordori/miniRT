/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_graphics.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 20:12:40 by myli-pen          #+#    #+#             */
/*   Updated: 2025/11/30 20:16:27 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_GRAPHICS_H
# define LIBFT_GRAPHICS_H

# include "libft_defs.h"
# include "libft_math.h"

struct s_vertex
{
	t_vec4		pos;
	t_vec4		o_pos;
	t_vec2i		s;
	uint32_t	color;
	float		depth;
};

struct s_transform
{
	t_vec3	pos;
	t_vec3	rot;
	t_vec3	scale;
};

struct s_quad
{
	float	topleft;
	float	topright;
	float	bottomleft;
	float	bottomright;
};

#endif
