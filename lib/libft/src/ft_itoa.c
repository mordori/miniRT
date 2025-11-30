/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:42:36 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/09 00:28:28 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft_utils.h"
#include "libft_str.h"

/**
 * Converts the signed integer `n` of `base` to a newly allocated string.
 * The base is validated.
 *
 * Handles negative numbers.
 *
 * @param n Signed integer to be converted.
 * @param base Base of the number.
 * @return String of the converted integer.
 */
char	*ft_itoa(int n, const char *base)
{
	char		*str;
	size_t		digits;
	size_t		base_len;
	long long	num;

	if (!ft_strchrdup(base))
		return (NULL);
	base_len = ft_strlen(base);
	num = n;
	if (n < 0)
		num = -num;
	digits = ft_countdigits(num, base_len) + (n < 0);
	str = malloc(digits + 1);
	if (!str)
		return (NULL);
	str[digits] = '\0';
	while (digits--)
	{
		str[digits] = base[num % base_len];
		num /= base_len;
	}
	if ((n < 0))
		str[0] = '-';
	return (str);
}
