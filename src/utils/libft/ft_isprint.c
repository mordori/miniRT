/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:22:39 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 17:57:33 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"

/**
 * Checks if `c` is a printable character in the standard ASCII character set.
 *
 * @param c Character to be checked for.
 * @return True if successful, else false.
 */
bool	ft_isprint(int c)
{
	c = (unsigned char)c;
	if (c >= ' ' && c <= '~')
		return (true);
	return (false);
}
