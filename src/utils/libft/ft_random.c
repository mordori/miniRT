/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_random.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:46:16 by myli-pen          #+#    #+#             */
/*   Updated: 2026/01/13 23:56:52 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_random.h"

// Xorshift32 by George Marsaglia
float	randomf(uint32_t *seed)
{
	*seed ^= *seed << 13;
	*seed ^= *seed >> 17;
	*seed ^= *seed << 5;
	return ((float)(*seed));
}

float	randomf01(uint32_t *seed)
{
	return (randomf(seed) / (float)UINT32_MAX);
}

float	randomfn11(uint32_t *seed)
{
	return ((randomf01(seed) - 0.5f) * 2.0f);
}

// https://old.reddit.com/r/RNG/comments/jqnq20/the_wang_and_jenkins_integer_hash_functions_just/
// Marc B. Reynolds
uint32_t	hash_lowerbias32(uint32_t seed)
{
	seed ^= seed >> 16;
	seed *= 0xa812d533;
	seed ^= seed >> 15;
	seed *= 0xb278e4ad;
	seed ^= seed >> 17;
	return (seed);
}
