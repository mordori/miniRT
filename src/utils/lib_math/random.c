/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:46:16 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/21 11:29:56 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

// Stateless mixer for seed hashing by Marc B. Reynolds
uint32_t	hash_lowerbias32(uint32_t seed)
{
	seed ^= seed >> 16;
	seed *= 0xa812d533;
	seed ^= seed >> 15;
	seed *= 0xb278e4ad;
	seed ^= seed >> 17;
	return (seed);
}

// Permutated Congruential Generator for sequences
uint32_t	pcg(uint32_t *state)
{
	uint32_t		old_state;
	uint32_t		word;

	old_state = *state;
	*state = old_state * 747796405u + 2891336453u;
	word = ((old_state >> ((old_state >> 28u) + 4u)) ^ old_state) * 277803737u;
	return ((word >> 22u) ^ word);
}

float	randomf(uint32_t *seed)
{
	return ((float)(pcg(seed)));
}

float	randomf01(uint32_t *seed)
{
	return (randomf(seed) / (float)UINT32_MAX);
}

float	randomfn11(uint32_t *seed)
{
	return ((randomf01(seed) - 0.5f) * 2.0f);
}
