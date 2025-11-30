/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 19:47:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 00:40:57 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# include <stdlib.h>

# include "MLX42.h"

# include "libft_defs.h"
# include "libft_vector.h"

# ifndef WIDTH
#  define WIDTH 1920
# endif

# ifndef HEIGHT
#  define HEIGHT 1080
# endif

typedef enum e_obj_type		t_obj_type;

typedef struct s_context	t_context;
typedef struct s_object		t_object;

enum e_obj_type
{
	PLANE,
	SPHERE,
	CYLINDER,
	CUBE,
	CONE,
	HYPERBOLOID,
	PARABOLOID
};

struct s_object
{
	t_obj_type	type;
};

struct s_context
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_vector	objs;
};

#endif
