/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:36:03 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 16:08:51 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"

/**
 * Converts lowercase alphabet character `a-z` to uppercase.
 *
 * @param c Character to be converted.
 * @return `A-Z`, otherwise `c`.
 */
int	ft_toupper(char *c)
{
	if (*c >= 'a' && *c <= 'z')
		*c -= 'a' - 'A';
	return (*c);
}
