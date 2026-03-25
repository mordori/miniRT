/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:19:53 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 19:19:55 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "libft_str.h"

bool	cmp_strs(const char **types, const char *src)
{
	while (*types)
	{
		if (!ft_strncmp(src, *types, ft_strlen(*types)))
			return (true);
		++types;
	}
	return (false);
}

size_t	uint64_to_str(uint64_t n, char *buf)
{
	char	temp[INT64_LENGTH];
	size_t	i;
	size_t	j;

	if (!n)
	{
		buf[0] = '0';
		return (1);
	}
	i = 0;
	while (n > 0)
	{
		temp[i++] = '0' + (n % 10);
		n /= 10;
	}
	j = 0;
	while (j < i)
	{
		buf[j] = temp[i - j - 1];
		++j;
	}
	return (i);
}
