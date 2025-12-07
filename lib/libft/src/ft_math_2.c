/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_math_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:39:37 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/07 05:06:55 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

float	ft_normalize(float value, float min, float max)
{
	return (ft_clamp01((value - min) / (max - min)));
}

float	ft_clamp01(float value)
{
	return (ft_clamp(value, 0.0f, 1.0f));
}

float	ft_clamp(float value, float min, float max)
{
	if (value <= min)
		value = min;
	else if (value >= max)
		value = max;
	return (value);
}

float	degrees_to_rad(float degrees)
{
	return (degrees * M_PI / 180.0f);
}
