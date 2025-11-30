/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:11:36 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 17:43:56 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_mem.h"

/**
 * Erases `n` bytes of data in the memory area `s` by writing `/0`.
 *
 * @param s Source memory area.
 * @param n Number of bytes to be NUL-terminated.
 * @return Pointer to modified `s`.
 */
void	*ft_bzero(void *s, size_t n)
{
	return (ft_memset(s, 0, n));
}
