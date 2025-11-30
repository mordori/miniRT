/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:51:45 by myli-pen          #+#    #+#             */
/*   Updated: 2025/10/05 16:29:22 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_mem.h"

/**
 * Fills `n` bytes of the memory area `s` with byte `c`.
 *
 * @param s Source memory area.
 * @param c Byte to be written with.
 * @param n Number of bytes to be written.
 * @return Pointer to modified `s`.
 */
void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*d;

	if (!s && n)
		return (NULL);
	d = (unsigned char *)s;
	while (n--)
		*d++ = (unsigned char)c;
	return (s);
}
