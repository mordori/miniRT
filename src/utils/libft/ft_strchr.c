/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:49:44 by myli-pen          #+#    #+#             */
/*   Updated: 2025/10/29 00:05:24 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "libft_str.h"

/**
 * Attempts to find the first occurrance of character `c` in `s`.
 *
 * @param s Source string.
 * @param c Character to be searched for.
 * @return Pointer to the first occurrence of `c` in `s`,
 * or `NULL` if not found.
 */
char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	c = (unsigned char)c;
	while (*s && (const unsigned char)*s != c)
		++s;
	if ((const unsigned char)*s == c)
		return ((char *)s);
	return (NULL);
}
