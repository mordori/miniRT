/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 00:22:08 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/12 01:03:21 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_vector.h"

/**
 * Returns the last item in `vec`.
 *
 * @param vec Vector to be operated.
 * @return Item if successful, else NULL.
 */
void	*vector_getlast(t_vector *vec)
{
	if (!vec || !vec->total)
		return (NULL);
	return (vec->items[vec->total - 1]);
}

void	vector_clean_items(t_vector *vec, void (*del)(void *))
{
	size_t	i;
	void	*temp;

	if (!vec || !del)
		return ;
	i = 0;
	while (i < vec->total)
	{
		temp = vector_get(vec, i);
		del(temp);
		++i;
	}
}

bool	vector_remove(t_vector *vec, void *item)
{
	uint32_t	i;

	if (!vec || !vec->total)
		return (false);
	i = 0;
	while (i < vec->total)
	{
		if (vec->items[i] == item)
			break;
		++i;
	}
	if (i == vec->total)
		return (false);
	while (i < vec->total - 1)
	{
		vec->items[i] = vec->items[i + 1];
		++i;
	}
	vec->total--;
	return (true);
}
