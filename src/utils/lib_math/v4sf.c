/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v4sf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:44:14 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/01 12:48:15 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_v4sf	v4sf(const float x, const float y, const float z, const float w)
{
	t_v4sf	res;

	res = (t_v4sf){x, y, z, w};
	return (res);
}

t_v4sf	v4sf_n(const float n)
{
	t_v4sf	res;

	res = (t_v4sf){n, n, n, n};
	return (res);
}
