/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:49:20 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/15 18:44:56 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"
#include "libft_utils.h"

int ft_atoi(char** str) {
	while (ft_isspace(**str))
		(*str)++;

	int sign = 1;
	if (**str == '-') {
		sign = -1;
		(*str)++;
	} else if (**str == '+') {
		(*str)++;
	}

	int number = 0;
	while (ft_isdigit(**str)) {
		number = number * 10 + (**str - '0');
		(*str)++;
	}
	return sign * number;
}

int64_t ft_strtol(char* str, char* end) {
	int sign;
	int64_t number;

	if (!str)
		return (ERROR);
	while (ft_isspace(*str))
		++str;
	sign = 1;
	if ((*str == '-' || *str == '+') && *str++ == '-')
		sign = -1;
	*end = 'e';
	number = 0;
	while (ft_isdigit(*str)) {
		number = number * 10 + (*str++ - '0');
		*end = *str;
	}
	return (sign * number);
}

int ft_atoi_base(const char* str, const char* base) {
	int sign;
	int result;
	size_t len_base;
	char* ptr;

	if (!str || !ft_strchrdup(base))
		return (ERROR);
	while (ft_isspace(*str))
		++str;
	sign = 1;
	len_base = ft_strlen(base);
	if ((*str == '-' || *str == '+') && *str++ == '-')
		sign = -1;
	result = 0;
	while (*str) {
		ptr = ft_strchr(base, *str);
		if (!ptr)
			break;
		result = result * len_base + (ptr - base);
		++str;
	}
	return (sign * result);
}

uint32_t ft_atouint32_t_base(const char* str, const char* base) {
	uint32_t result;
	size_t len_base;
	char* ptr;

	if (!str || !ft_strchrdup(base))
		return (ERROR);
	while (ft_isspace(*str))
		++str;
	len_base = ft_strlen(base);
	if (*str == '+')
		str++;
	result = 0;
	while (*str) {
		if (!*str)
			break;
		ptr = ft_strchr(base, *str);
		if (!ptr)
			return (ERROR_COLOR);
		result = result * len_base + (ptr - base);
		++str;
	}
	return (result);
}

bool ft_isspace(char c) {
	c = (unsigned char)c;
	return c == ' ' || (c >= '\t' && c <= '\r');
}
