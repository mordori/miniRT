/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_defs.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:23:22 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/16 00:06:39 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_DEFS_H
# define LIBFT_DEFS_H

# include <stdbool.h>
# include <float.h>
# include <stddef.h>
# include <stdint.h>
# include <limits.h>
# include <sys/types.h>
# include <errno.h>

# define ERROR			-1
# define GNL_ERROR		-1
# define GNL_EOF		0
# define GNL_OK			1

# define BASE_10		"0123456789"
# define BASE_16		"0123456789ABCDEF"
# define INT32_LENGTH	11
# define INT64_LENGTH	20

# define RW_RW_RW_		0666
# define RW_______		0600

# define WHITE			0xFFFFFFFF
# define BLACK			0x000000FF
# define RED			0xFF0000FF
# define GREEN			0x00FF00FF
# define BLUE			0x0000FFFF
# define ERROR_COLOR	0xFF00FFFF

# ifndef VECTOR_SIZE
#  define VECTOR_SIZE	4
# endif

# ifndef MEM_UNIT
#  define MEM_UNIT		1024UL
# endif
# ifndef MEMORY
#  define MEMORY		16777216UL
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE	1024
# endif

# ifndef FD_MAX
#  define FD_MAX		4
# endif

# ifndef M_PI
#  define M_PI			3.14159265358979323846
# endif

# ifndef M_PI_2
#  define M_PI_2		1.57079632679489661923
# endif

# ifndef M_INF
// FLT_MAX interpret as infinity when using --ffast-math flag
#  define M_INF			FLT_MAX
# endif

typedef struct s_list		t_list;
typedef struct s_vec2i		t_vec2i;
typedef struct s_vec2i		t_int2;
typedef struct s_mat4		t_mat4;
typedef struct s_quad		t_quad;
typedef struct s_vertex		t_vertex;
typedef struct s_transform	t_transform;
typedef struct s_arena		t_arena;
typedef struct s_ray		t_ray;
typedef struct s_hit		t_hit;

typedef union u_vec2		t_point;
typedef union u_vec2		t_float2;
typedef union u_vec2		t_vec2;
typedef union u_vec3		t_vec3;
typedef union u_vec3		t_float3;
typedef union u_vec4		t_vec4;
typedef union u_vec4		t_float4;
typedef union u_color		t_color;
typedef union u_vec4		t_quaternion;

#endif
