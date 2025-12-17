/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 21:37:39 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 15:44:09 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"

/**
 * Allocates memory and duplicates string `s`.
 *
 * @param s Source string.
 * @return New string duplicated from `s`.
 */
char	*ft_strdup(const char *s)
{
	if (!s)
		return (NULL);
	return (ft_substr(s, 0, ft_strlen(s)));
}
