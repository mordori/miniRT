/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:47:55 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 15:20:43 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_mem.h"

/**
 * Compares `n` bytes of the memory areas `s1` and `s2`.
 *
 * @param s1 Source 1 memory area.
 * @param s2 Source 2 memory area.
 * @param n Number of bytes to be written.
 * @return Integer value from the difference of the compared characters.
 */
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*str1;
	const unsigned char	*str2;

	if (!s1 || !s2 || !n)
		return (0);
	str1 = (const unsigned char *)s1;
	str2 = (const unsigned char *)s2;
	while (*str1 == *str2 && --n)
	{
		++str1;
		++str2;
	}
	return (*str1 - *str2);
}
