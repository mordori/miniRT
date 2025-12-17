/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:10:39 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 15:45:15 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"

/**
 * Tries to append characters from string `src` to the end of string `dst`.
 *
 * NUL-terminates the result.
 *
 * @param dst Destination string.
 * @param src Source string.
 * @param size Total amount of characters to be concatenated.
 * @return Returns the length of the string attempted to concatenate.
 */
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	if (!dst || !src)
		return (0);
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dst_len >= size)
		return (src_len + size);
	i = 0;
	while (dst_len + i < size - 1 && src[i])
	{
		dst[dst_len + i] = src[i];
		++i;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}
