/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mat3_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 06:33:41 by myli-pen          #+#    #+#             */
/*   Updated: 2026/01/29 06:43:24 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_matrix.h"

t_vec3	mat3_mul_vec3(t_mat3 m, t_vec3 v)
{
	t_vec3	result;

	result.x = v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2];
	result.y = v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2];
	result.z = v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2];
	return (result);
}
