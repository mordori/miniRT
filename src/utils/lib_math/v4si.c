/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v4si.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 22:47:34 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/01 22:49:13 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_v4si	v4si(const int32_t x, const int32_t y, const int32_t z, const int32_t w)
{
	t_v4si	res;

	res = (t_v4si){x, y, z, w};
	return (res);
}

t_v4si	v4si_n(const int32_t n)
{
	t_v4si	res;

	res = (t_v4si){n, n, n, n};
	return (res);
}
