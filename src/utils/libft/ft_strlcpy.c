/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:19:27 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 15:45:20 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"

/**
 * Copies up to `size-1` characters from the string `src` to the string `dst`.
 *
 * NUL-terminates the result.
 *
 * @param dst Destination string.
 * @param src Source string.
 * @param size Amount of characters to be copied.
 * @return Returns the length of the string `src` attempted to copy.
 */
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;

	len = ft_strlen(src);
	if (!dst || !src || !size)
		return (len);
	while (--size && *src)
		*dst++ = *src++;
	*dst = '\0';
	return (len);
}
