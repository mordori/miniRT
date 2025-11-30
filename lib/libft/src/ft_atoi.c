/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:49:20 by myli-pen          #+#    #+#             */
/*   Updated: 2025/09/24 18:47:30 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"
#include "libft_str.h"

/**
 * Converts the initial portion of the string `str` to an integer value.
 *
 * Does not handle overflow.
 *
 * In case of NULL `nptr` returns arbitrary -1.
 *
 * @param str Source string.
 * @return Integer value converted from the initial portion of the provided
 * string `str`.
 */
int	ft_atoi(const char *str)
{
	int	sign;
	int	number;

	if (!str)
		return (ERROR);
	while (ft_isspace(*str))
		++str;
	sign = 1;
	if ((*str == '-' || *str == '+') && *str++ == '-')
		sign = -1;
	number = 0;
	while (ft_isdigit(*str))
		number = number * 10 + (*str++ - '0');
	return (sign * number);
}

int64_t	ft_strtol(char *str, char *end)
{
	int		sign;
	int64_t	number;

	if (!str)
		return (ERROR);
	while (ft_isspace(*str))
		++str;
	sign = 1;
	if ((*str == '-' || *str == '+') && *str++ == '-')
		sign = -1;
	*end = 'e';
	number = 0;
	while (ft_isdigit(*str))
	{
		number = number * 10 + (*str++ - '0');
		*end = *str;
	}
	return (sign * number);
}

int	ft_atoi_base(const char *str, const char *base)
{
	int		sign;
	int		result;
	size_t	len_base;
	char	*ptr;

	if (!str || !ft_strchrdup(base))
		return (ERROR);
	while (ft_isspace(*str))
		++str;
	sign = 1;
	len_base = ft_strlen(base);
	if ((*str == '-' || *str == '+') && *str++ == '-')
		sign = -1;
	result = 0;
	while (*str)
	{
		ptr = ft_strchr(base, *str);
		if (!ptr)
			break ;
		result = result * len_base + (ptr - base);
		++str;
	}
	return (sign * result);
}

uint32_t	ft_atouint32_t_base(const char *str, const char *base)
{
	uint32_t	result;
	size_t		len_base;
	char		*ptr;

	if (!str || !ft_strchrdup(base))
		return (ERROR);
	while (ft_isspace(*str))
		++str;
	len_base = ft_strlen(base);
	if (*str == '+')
		str++;
	result = 0;
	while (*str)
	{
		if (!*str)
			break ;
		ptr = ft_strchr(base, *str);
		if (!ptr)
			return (ERROR_COLOR);
		result = result * len_base + (ptr - base);
		++str;
	}
	return (result);
}

/**
 * Checks if `c` is a whitespace character.
 *
 * @param c Character to be checked for.
 * @return TRUE if successful, else FALSE.
 */
bool	ft_isspace(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}
