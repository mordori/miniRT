/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:49:28 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:49:31 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"
#include "libft_utils.h"
#include <float.h>

static float	skip_whitespace_and_sign(const char **str)
{
	while (ft_isspace(**str))
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
		{
			(*str)++;
			return (-1.0f);
		}
		(*str)++;
	}
	return (1.0f);
}

static float	parse_integer_part(const char **str)
{
	float	result;
	float	digit;

	result = 0.0f;
	while (ft_isdigit(**str))
	{
		digit = *(*str)++ - '0';
		if (result > (FLT_MAX - digit) / 10.0f)
		{
			while (ft_isdigit(**str))
				(*str)++;
			return (FLT_MAX);
		}
		result = result * 10.0f + digit;
	}
	return (result);
}

static float	parse_fraction(const char **str)
{
	float	fraction;
	float	weight;
	int		count;

	fraction = 0.0f;
	weight = 0.1f;
	count = 0;
	(*str)++;
	while (ft_isdigit(**str))
	{
		count++;
		if (count > 9)
			break ;
		fraction += (**str - '0') * weight;
		weight *= 0.1f;
		(*str)++;
	}
	return (fraction);
}

static void	set_end_pointer(const char *start, const char *current,
		char **endptr)
{
	if (!endptr || !start || !current)
		return ;
	if (current == start || (current == start + 1 && (*start == '-'
				|| *start == '+')))
		*endptr = (char *)start;
	else
		*endptr = (char *)current;
}

float	ft_atof(const char *str, char **endptr)
{
	float		result;
	float		sign;
	const char	*start;

	if (!str || !endptr)
		return (0.0f);
	start = str;
	sign = skip_whitespace_and_sign(&str);
	result = parse_integer_part(&str);
	if (*str == '.' && !ft_isdigit(*(str + 1)))
	{
		*endptr = (char *)str;
		return (result * sign);
	}
	if (*str == '.' && ft_isdigit(*(str + 1)))
		result += parse_fraction(&str);
	set_end_pointer(start, str, endptr);
	return (result * sign);
}
