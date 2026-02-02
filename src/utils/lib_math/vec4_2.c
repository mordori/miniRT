/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 23:32:25 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/01 12:09:48 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec4	vec4_3(const t_vec3 vec, const float w)
{
	t_vec4		result;

	result.v = vec.v;
	result.w = w;
	return (result);
}
