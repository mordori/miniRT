/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2i_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:05:21 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/15 21:27:13 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec2i	vec2i_f(float x, float y)
{
	return ((t_vec2i){(int32_t)x, (int32_t)y});
}

t_vec2i	vec2i_nf(float n)
{
	return ((t_vec2i){(int32_t)n, (int32_t)n});
}
