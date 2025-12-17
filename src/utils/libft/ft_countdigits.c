/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_countdigits.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:21:49 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 18:54:28 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"

/**
 * Counts the number of digits in `n` with a specified base.
 *
 * @param n Signed integer number.
 * @param len Number of characters in the base used.
 * @return Number of digits in an integer number.
 */
size_t	ft_countdigits(long long n, const size_t len)
{
	size_t		count;
	long long	base;

	if (!len)
		return (ERROR);
	base = (long long)len;
	count = 1;
	n /= base;
	while (n)
	{
		++count;
		n /= base;
	}
	return (count);
}

/**
 * Counts the number of digits in `n` with a specified base.
 *
 * @param n Unsigned integer number.
 * @param len Number of characters in the base used.
 * @return Number of digits in an integer number.
 */
size_t	ft_ucountdigits(uintptr_t n, const size_t len)
{
	size_t	count;

	if (!len)
		return (ERROR);
	count = 1;
	n /= len;
	while (n)
	{
		++count;
		n /= len;
	}
	return (count);
}
