/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:58:06 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 15:56:33 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"

/**
 * Locates string `little` in string `big`,
 * where up to `len` characters are searched.
 *
 * @param big Source string.
 * @param little Substring searched.
 * @param len Maximum amount of characters searched in `big`.
 * @return Pointer to the beginning of substring `little` in string `big`,
 * or `NULL` if not found.
 */
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	start;
	size_t	i;

	if (!big || !little)
		return (NULL);
	if (!*little)
		return ((char *)big);
	start = 0;
	while (big[start])
	{
		i = 0;
		while (big[start + i] == little[i] && start + i < len)
			++i;
		if (!little[i])
			return ((char *)&big[start]);
		++start;
	}
	return (NULL);
}
