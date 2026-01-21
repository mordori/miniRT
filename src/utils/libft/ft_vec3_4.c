/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec3_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 18:35:30 by myli-pen          #+#    #+#             */
/*   Updated: 2026/01/21 18:39:15 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

t_vec3	vec3_sqrt(t_vec3 vec)
{
	return ((t_vec3){{
		sqrtf(vec.r),
		sqrtf(vec.g),
		sqrtf(vec.b)
	}});
}

t_vec3	vec3_clamp01(t_vec3 vec)
{
	return ((t_vec3){{
		ft_clamp01(vec.r),
		ft_clamp01(vec.g),
		ft_clamp01(vec.b)
	}});
}
