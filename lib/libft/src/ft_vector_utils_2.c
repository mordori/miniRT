/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 00:22:08 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/09 00:22:17 by myli-pen         ###   ########.fr       */
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
