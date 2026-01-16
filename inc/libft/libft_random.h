/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_random.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:44:52 by myli-pen          #+#    #+#             */
/*   Updated: 2026/01/16 01:12:54 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_RANDOM_H
# define LIBFT_RANDOM_H

# include "libft_defs.h"

uint32_t	random_uint32(uint32_t *seed);
float	randomf(uint32_t *seed);
float	randomf01(uint32_t *seed);
float	randomfn11(uint32_t *seed);
uint32_t	hash_lowerbias32(uint32_t seed);

#endif
