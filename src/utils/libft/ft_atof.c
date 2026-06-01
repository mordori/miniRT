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

#include "libft_utils.h"

float ft_atof(char** str) {
	while (ft_isspace(**str))
		(*str)++;

	float sign = 1.0f;
	if (**str == '-') {
		sign = -1.0f;
		(*str)++;
	} else if (**str == '+') {
		(*str)++;
	}

	float number = 0.0f;
	while (ft_isdigit(**str)) {
		number = number * 10.0 + (float)(**str - '0');
		(*str)++;
	}

	if (**str == '.') {
		(*str)++;
		float fraction = 0.1f;
		while (ft_isdigit(**str)) {
			number += (float)(**str - '0') * fraction;
			fraction *= 0.1f;
			(*str)++;
		}
	}
	return sign * number;
}
