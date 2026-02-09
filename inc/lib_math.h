/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_math.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:11:20 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/08 17:11:38 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_MATH_H
# define LIB_MATH_H

# define _GNU_SOURCE

# include <math.h>

# include <stdbool.h>
# include <float.h>
# include <stddef.h>
# include <stdint.h>
# include <limits.h>
# include <sys/types.h>
# include <xmmintrin.h>

# define M_TAU				6.28318530717958647693f
# define M_INF				FLT_MAX

# define M_1_255F			0.003921568627451f
# define M_1_2P2F			0.454545454545454f
# define M_1_2PI			0.15915494309189533577f

# define M_EPSILON			1e-6f
# define LEN_EPSILON		1e-6f
# define LEN_SQ_EPSILON		1e-12f
# define B_EPSILON			1e-4f
# define G_EPSILON			1e-5f

typedef struct s_vec2i		t_vec2i;
typedef struct s_vec2i		t_int2;
typedef struct s_vec2ui		t_uint2;
typedef struct s_mat2		t_mat2;
typedef struct s_mat3		t_mat3;
typedef struct s_mat4		t_mat4;

typedef union u_f_ui		t_f_ui;
typedef union u_vec2		t_point;
typedef union u_vec2		t_float2;
typedef union u_vec2		t_vec2;
typedef union u_vec3		t_vec3;
typedef union u_vec4		t_vec4;
typedef union u_vec3		t_float3;
typedef union u_vec4		t_float4;
typedef union u_color		t_color;
typedef union u_vec4		t_quaternion;

typedef __attribute__((vector_size(16))) float		t_v4sf;
typedef __attribute__((vector_size(16))) int32_t	t_v4si;
typedef __attribute__((vector_size(16))) uint32_t	t_v4ui;

struct __attribute__((aligned(16))) s_mat2
{
	float	m[2][2];
};

struct __attribute__((aligned(16))) s_mat3
{
	float	m[3][3];
};

struct __attribute__((aligned(16))) s_mat4
{
	union
	{
		float	m[4][4];
		t_v4sf	rows[4];
	};
};

union u_f_ui
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
	struct
	{
		float	sin;
		float	cos;
	};
};

union __attribute__((aligned(16))) u_vec3
{
	t_v4sf	v;
	float	data[4];
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
	t_v4sf	v;
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

union u_color
{
	uint32_t	full;
	struct
	{
		uint8_t	r;
		uint8_t	g;
		uint8_t	b;
		uint8_t	a;
	};
};

int			ft_imax(int a, int b);
int			ft_imin(int a, int b);
float		ft_lerp(float a, float b, float t);
float		ft_lerp_fast(float a, float b, float t);
float		ft_normalize(float value, float min, float max);
float		clampf01(float value);
float		clampf(float value, float min, float max);
float		f_schlick(float u, float f0, float f90);
float		degrees_to_rad(float degrees);
bool		ft_is_pot(size_t n);
size_t		ft_pow(size_t n, size_t e);
uint32_t	ft_uint_min(uint32_t a, uint32_t b);

// mat3
// -----------------------------------------------------------------
float		mat3_det(const t_mat3 *src);

// mat4
// -----------------------------------------------------------------
bool		mat4_inverse(const t_mat4 *src, t_mat4 *out);
t_mat3		mat4_sub(const t_mat4 *src, int skip_r, int skip_c);
float		mat4_cofactor(const t_mat4 *src, int row, int col);
float		mat4_det(const t_mat4 *src);
t_mat4		mat4_identity(void);
t_mat4		mat4_transpose(const t_mat4 *m);
t_mat4		mat4_mul(const t_mat4 *a, const t_mat4 *b);
t_vec4		mat4_mul_vec4(const t_mat4 *m, t_vec4 v);
t_mat4		mat4_translate(t_vec3 t);
t_mat4		mat4_scale(t_vec3 s);
t_mat4		mat4_rot_x(float rad);
t_mat4		mat4_rot_y(float rad);
t_mat4		mat4_rot_z(float rad);
t_vec3		mat4_mul_vec3(const t_mat4 *m, t_vec3 v);
t_mat4		mat4_rot(t_vec3 rot);

// vec2i
// -----------------------------------------------------------------
t_vec2i		vec2i(int32_t x, int32_t y);
t_vec2i		vec2i_n(int32_t n);
t_vec2i		vec2i_f(float x, float y);
t_vec2i		vec2i_nf(float n);
t_vec2i		vec2i_add(t_vec2i a, t_vec2i b);
t_vec2i		vec2i_sub(t_vec2i a, t_vec2i b);
t_vec2i		vec2i_scale(t_vec2i vec, float s);

// vec2
// -----------------------------------------------------------------
t_vec2		vec2(float x, float y);
t_vec2		vec2_n(float n);
t_vec2		vec2_add(t_vec2 a, t_vec2 b);
t_vec2		vec2_sub(t_vec2 a, t_vec2 b);
t_vec2		vec2_scale(t_vec2 vec, float s);

// vec3
// -----------------------------------------------------------------
t_vec3		vec3(float x, float y, float z);
t_vec3		vec3_n(float n);
t_vec3		vec3_add(t_vec3 a, t_vec3 b);
t_vec3		vec3_sub(t_vec3 a, t_vec3 b);
t_vec3		vec3_scale(t_vec3 vec, float s);
t_vec3		vec3_cross(t_vec3 a, t_vec3 b);
float		vec3_dot(t_vec3 a, t_vec3 b);
t_vec3		vec3_div(t_vec3 vec, float div);
float		vec3_length(t_vec3 vec);
t_vec3		vec3_normalize(t_vec3 vec);
t_vec3		vec3_reflect(t_vec3 vec, t_vec3 n);
t_vec3		vec3_unit_random(uint32_t *seed);
t_vec3		vec3_min(t_vec3 vec, float min);
t_vec3		vec3_max(t_vec3 vec, float max);
t_vec3		vec3_sqrt(t_vec3 vec);
t_vec3		vec3_clamp01(t_vec3 vec);
t_vec3		vec3_clamp_mag(t_vec3 vec, float max);
t_vec3		vec3_add_n(t_vec3 vec, float n);
t_vec3		vec3_mul(t_vec3 a, t_vec3 b);
t_vec3		vec3_negate(t_vec3 vec);
t_vec3		vec3_clamp(t_vec3 vec, float min, float max);
t_vec3		vec3_lerp(t_vec3 a, t_vec3 b, float t);
t_vec3		vec3_schlick(t_vec3 f0, float u);

// vec4
// -----------------------------------------------------------------
t_vec4		vec4(float x, float y, float z, float w);
t_vec4		vec4_3(t_vec3 vec, float w);
t_vec4		vec4_n(float n);
t_vec4		vec4_add(t_vec4 a, t_vec4 b);
t_vec4		vec4_sub(t_vec4 a, t_vec4 b);
t_vec4		vec4_scale(t_vec4 vec, float s);

// v4sf, v4si
// -----------------------------------------------------------------
t_v4sf		v4sf(float x, float y, float z, float w);
t_v4sf		v4sf_n(float n);
t_v4si		v4si(int32_t x, int32_t y, int32_t z, int32_t w);
t_v4si		v4si_n(int32_t n);

// Random
// -----------------------------------------------------------------
uint32_t	pcg(uint32_t *state);
float		randomf(uint32_t *seed);
float		randomf01(uint32_t *seed);
float		randomfn11(uint32_t *seed);
uint32_t	hash_lowerbias32(uint32_t seed);

// Colors
// -----------------------------------------------------------------
t_vec3		lerp_color(uint32_t color1, uint32_t color2, float t);
uint32_t	rgba_to_abgr(uint32_t rgba);

#endif
