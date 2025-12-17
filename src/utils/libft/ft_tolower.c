/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:40:50 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 16:08:57 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"

/**
 * Converts uppercase alphabet character `A-Z` to lowercase.
 *
 * @param c Character to be converted.
 * @return `a-z`, otherwise `c`.
 */
int	ft_tolower(char *c)
{
	if (*c >= 'A' && *c <= 'Z')
		*c += 'a' - 'A';
	return (*c);
}
