/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v4si.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 22:47:34 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/06 04:29:25 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_v4si	v4si(int32_t x, int32_t y, int32_t z, int32_t w)
{
	t_v4si	res;

	res = (t_v4si){x, y, z, w};
	return (res);
}

t_v4si	v4si_n(int32_t n)
{
	t_v4si	res;

	res = (t_v4si){n, n, n, n};
	return (res);
}
