/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtof.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:21 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <float.h>

#include "libft_utils.h"
#include "parsing.h"

static float skip_whitespace_and_sign(const char** str) {
	while (ft_isspace(**str))
		(*str)++;
	if (**str == '-' || **str == '+') {
		if (**str == '-') {
			(*str)++;
			return (-1.0f);
		}
		(*str)++;
	}
	return 1.0f;
}

static float parse_integer_part(const char** str) {
	float result = 0.0f;
	int count = 0;
	while (ft_isdigit(**str)) {
		count++;
		if (count > 10)
			break;

		float digit = *(*str)++ - '0';
		if (result > (FLT_MAX - digit) / 10.0f) {
			while (ft_isdigit(**str))
				(*str)++;
			return FLT_MAX;
		}
		result = result * 10.0f + digit;
	}
	return result;
}

static float parse_fraction(const char** str) {
	float fraction = 0.0f;
	float weight = 0.1f;
	int count = 0;
	(*str)++;
	while (ft_isdigit(**str)) {
		count++;
		if (count > 10)
			break;

		fraction += (**str - '0') * weight;
		weight *= 0.1f;
		(*str)++;
	}
	return fraction;
}

static void set_end_pointer(const char* start, const char* current, char** endptr) {
	if (!endptr || !start || !current)
		return;
	if (current == start || (current == start + 1 && (*start == '-' || *start == '+')))
		*endptr = (char*)start;
	else
		*endptr = (char*)current;
}

float ft_strtof(const char* str, char** endptr) {
	if (!str || !endptr)
		return 0.0f;

	const char* start = str;
	float sign = skip_whitespace_and_sign(&str);
	float result = parse_integer_part(&str);
	if (*str == '.' && !ft_isdigit(*(str + 1))) {
		*endptr = (char*)str;
		return result * sign;
	}
	if (*str == '.' && ft_isdigit(*(str + 1)))
		result += parse_fraction(&str);
	set_end_pointer(start, str, endptr);
	return result * sign;
}
