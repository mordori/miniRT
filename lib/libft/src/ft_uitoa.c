/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:19:31 by myli-pen          #+#    #+#             */
/*   Updated: 2025/10/02 01:24:04 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft_utils.h"
#include "libft_str.h"

/**
 * Converts the unsigned integer `n` of `base` to a newly allocated string.
 * The base is validated.
 *
 * @param n Unsigned integer to be converted.
 * @param base Base of the number.
 * @return String of the converted number.
 */
char	*ft_uitoa(uintptr_t n, const char *base)
{
	char	*str;
	size_t	digits;
	size_t	len;

	if (!ft_strchrdup(base))
		return (NULL);
	len = ft_strlen(base);
	digits = ft_ucountdigits(n, len);
	str = malloc((digits + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[digits] = '\0';
	while (digits--)
	{
		str[digits] = base[n % len];
		n /= len;
	}
	return (str);
}
