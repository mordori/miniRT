/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchrdup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:24:02 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 18:21:14 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"

/**
 * Checks if the string `str` has no duplicate characters.
 *
 * @param str Source string.
 * @return True if no duplicates are found, false otherwise.
 */
bool	ft_strchrdup(const char *str)
{
	int	i;
	int	j;

	if (!str || !*str)
		return (false);
	i = 0;
	while (str[i])
	{
		j = 0;
		while (j < i)
		{
			if (str[j] == str[i])
				return (false);
			++j;
		}
		++i;
	}
	return (true);
}
