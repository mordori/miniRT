/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:43:29 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 17:54:03 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"

/**
 * Checks if `c` is a numeric character.
 *
 * @param c Character to be checked for.
 * @return True if successful, else false.
 */
bool	ft_isdigit(int c)
{
	c = (unsigned char)c;
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}
