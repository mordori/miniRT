/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v4sf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:44:14 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/06 04:29:14 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_v4sf	v4sf(float x, float y, float z, float w)
{
	t_v4sf	res;

	res = (t_v4sf){x, y, z, w};
	return (res);
}

t_v4sf	v4sf_n(float n)
{
	t_v4sf	res;

	res = (t_v4sf){n, n, n, n};
	return (res);
}
