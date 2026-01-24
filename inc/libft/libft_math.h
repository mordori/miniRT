/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_math.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:11:20 by myli-pen          #+#    #+#             */
/*   Updated: 2026/01/24 04:35:01 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MATH_H
# define LIBFT_MATH_H

# include <math.h>

# include "libft_defs.h"

union u_f_int
{
	float		f;
	uint32_t	i;
};

struct s_vec2i
{
	int32_t		x;
	int32_t		y;
};

struct s_vec2ui
{
	uint32_t	x;
	uint32_t	y;
};

union u_vec2
{
	struct
	{
		float	x;
		float	y;
	};
	struct
	{
		float	u;
		float	v;
	};
};

union __attribute__((aligned(16))) u_vec3
{
	float	data[4];		// Extra element optimizes u_vec3 to 16 bytes
	struct
	{
		float	x;
		float	y;
		float	z;
	};
	struct
	{
		float	r;
		float	g;
		float	b;
	};
	struct
	{
		float	right;
		float	up;
		float	forward;
	};
	t_vec2	xy;
};

union __attribute__((aligned(16))) u_vec4
{
	float	data[4];
	struct
	{
		float	x;
		float	y;
		float	z;
		float	w;
	};
	struct
	{
		float	r;
		float	g;
		float	b;
		float	a;
	};
	t_vec3	xyz;
	t_vec3	rgb;
};

float	ft_lerp(float a, float b, float t);
int		ft_imax(int a, int b);
int		ft_imin(int a, int b);
float	ft_normalize(float value, float min, float max);
float	ft_clamp01(float value);
float	ft_clamp(float value, float min, float max);
t_vec2i	vec2i(int32_t x, int32_t y);
t_vec2i	vec2i_n(int32_t n);
t_vec2i	vec2i_f(float x, float y);
t_vec2i	vec2i_nf(float n);
t_vec2i	vec2i_add(t_vec2i a, t_vec2i b);
t_vec2i	vec2i_sub(t_vec2i a, t_vec2i b);
t_vec2i	vec2i_scale(t_vec2i vec, float s);
t_vec2	vec2(float x, float y);
t_vec2	vec2_n(float n);
t_vec2	vec2_add(t_vec2 a, t_vec2 b);
t_vec2	vec2_sub(t_vec2 a, t_vec2 b);
t_vec2	vec2_scale(t_vec2 vec, float s);
t_vec3	vec3(float x, float y, float z);
t_vec3	vec3_n(float n);
t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_sub(t_vec3 a, t_vec3 b);
t_vec3	vec3_scale(t_vec3 vec, float s);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
float	vec3_dot(t_vec3 a, t_vec3 b);
t_vec3	vec3_div(t_vec3 vec, float div);
float	vec3_length(t_vec3 vec);
t_vec3	vec3_normalize(t_vec3 vec);
t_vec3	vec3_reflect(t_vec3 vec, t_vec3 n);
t_vec3	vec3_unit_random(uint32_t *seed);
t_vec3	vec3_min(t_vec3 vec, float min);
t_vec3	vec3_max(t_vec3 vec, float max);
t_vec3	vec3_sqrt(t_vec3 vec);
t_vec3	vec3_clamp01(t_vec3 vec);
t_vec3	vec3_clamp_mag(t_vec3 vec, float max);
t_vec4	vec4(float x, float y, float z, float w);
t_vec4	vec4_3(t_vec3 vec, float w);
t_vec4	vec4_n(float n);
t_vec4	vec4_add(t_vec4 a, t_vec4 b);
t_vec4	vec4_sub(t_vec4 a, t_vec4 b);
t_vec4	vec4_scale(t_vec4 vec, float s);
size_t	ft_pow(size_t n, size_t e);
bool	ft_is_pot(size_t n);
float	degrees_to_rad(float degrees);
t_vec3	vec3_mul(t_vec3 a, t_vec3 b);
uint32_t	ft_uint_min(uint32_t a, uint32_t b);

#endif
