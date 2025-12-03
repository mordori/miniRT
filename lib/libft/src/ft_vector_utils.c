/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:13:36 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 22:23:21 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_vector.h"
#include "libft_mem.h"

/**
 * Creates a new items array and replaces the old one.
 *
 * @param vec Vector to be operated.
 * @param size Capacity of the new items array to be allocated.
 * @return True if successful, else false.
 */
static inline bool	vector_resize(t_vector *vec, size_t size)
{
	void	**items;

	items = malloc(sizeof(void *) * size);
	if (!items)
		return (false);
	ft_memcpy(items, vec->items, sizeof(void *) * vec->total);
	free(vec->items);
	vec->items = items;
	vec->size = size;
	return (true);
}

/**
 * Adds `new` to the items of `vec`.
 *
 * Resizes the items array if needed.
 *
 * @param vec Vector to be operated.
 * @param new New item to be added.
 * @return True if successful, else false.
 */
bool	vector_add(t_vector *vec, void *new)
{
	if (!vec || !vec->size)
		return (false);
	if (vec->total == vec->size)
		if (!vector_resize(vec, vec->size * 2))
			return (false);
	vec->items[vec->total++] = new;
	return (true);
}

bool	vector_insert(t_vector *vec, void *new, size_t index)
{
	size_t	i;

	if (!vec || !vec->size || index > vec->total)
		return (false);
	if (vec->total == vec->size)
		if (!vector_resize(vec, vec->size * 2))
			return (false);
	i = vec->total;
	while (i > index)
	{
		vec->items[i] = vec->items[i - 1];
		--i;
	}
	vec->items[index] = new;
	vec->total++;
	return (true);
}

/**
 * Removes an item with the `index` in `vec`.
 *
 * Resizes the items array if needed.
 *
 * Frees the item, if it belongs to heap.
 *
 * @param vec Vector to be operated.
 * @param index Index of the item to be removed.
 * @return True if successful, else false.
 */
bool	vector_del(t_vector *vec, size_t index)
{
	if (!vec || !vec->total || index >= vec->total)
		return (false);
	if (vec->del)
	{
		vec->del(vec->items[index]);
		vec->items[index] = NULL;
	}
	while (index < vec->total - 1)
	{
		vec->items[index] = vec->items[index + 1];
		++index;
	}
	vec->total--;
	if (vec->is_shrink && vec->size > VECTOR_SIZE && vec->total > 0 && \
vec->total == vec->size / 4)
		if (!vector_resize(vec, vec->size / 2))
			return (false);
	return (true);
}

/**
 * Frees the items array and cleans ´vec´.
 *
 * Additionally frees all item elements, if they belong to heap.
 *
 * @param vec Vector to be operated.
 * @return True if successful, else false.
 */
bool	vector_free(t_vector *vec, ...)
{
	size_t	i;
	va_list	params;

	if (!vec)
		return (false);
	va_start(params, vec);
	while (vec)
	{
		if (vec->size)
		{
			i = 0;
			if (vec->del && vec->total)
			{
				while (i < vec->total)
					vec->del(vec->items[i++]);
			}
			vec->total = 0;
			vec->size = 0;
			free(vec->items);
			vec->items = NULL;
		}
		vec = va_arg(params, t_vector *);
	}
	va_end(params);
	return (true);
}
