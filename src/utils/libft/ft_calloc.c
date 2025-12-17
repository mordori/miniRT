/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:00:43 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 17:45:13 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>

#include "libft_mem.h"

/**
 * Allocates memory for `nmemb` elements of `size` bytes, initialized with \0.
 *
 * Guards against overflow, returning NULL.
 *
 * @param nmemb Number of elements.
 * @param size Size of an element in bytes.
 * @return Pointer to the beginning of the allocated memory area, or NULL
 * if allocation failed.
 */
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (size && nmemb > SIZE_MAX / size)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	return (ft_bzero(ptr, nmemb * size));
}
