/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 20:20:54 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 18:20:16 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"

/**
 * Applies function `f` to each character of string `s`.
 *
 * @param s Source string
 * @param f Fuction pointer
 * @return True if successful, else false.
 */
bool	ft_striteri(char *s, int (*f)(char*))
{
	unsigned int	i;

	if (!s || !f)
		return (false);
	i = 0;
	while (s[i])
		f(&s[i++]);
	return (true);
}
