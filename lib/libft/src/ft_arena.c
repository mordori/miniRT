/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arena.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 20:50:47 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 00:14:51 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft_arena.h"
#include "libft_mem.h"
#include "libft_math.h"

/**
 * @brief	Allocates and zero-initializes memory for the arena and initializes
 * its variables.
 *
 * @param	capacity Total bytes the arena can contain.
 *
 * @return	Created memory arena or NULL if creation fails.
 */
t_arena	arena_create(\
void *ctx, size_t capacity, void (*e)(void *ctx, char *msg))
{
	t_arena	arena;

	if (capacity < MEM_UNIT)
		f(ctx, "arena capacity is less than 1 KiB");
	if (!ft_is_pot(capacity))
		f(ctx, "arena capacity is not a power of 2");
	arena.base = ft_calloc(capacity, 1);
	if (!arena.base)
		f(ctx, "arena malloc failed");
	arena.capacity = capacity;
	arena.head = 0;
	return (arena);
}

/**
 * @brief	Reserves a block of memory from the arena and moves `head` index.
 *
 * @param	arena Pointer to the arena.
 * @param	size Amount of bytes to reserve from the arena.
 *
 * @return	Pointer to the block of memory that was reserved.
 */
void	*arena_alloc(t_arena *arena, size_t size)
{
	void	*ptr;

	if (arena->head + size > arena->capacity)
		return (NULL);
	ptr = arena->base + arena->head;
	arena->head += size;
	return (ptr);
}

/**
 * @brief	Resets the `head` index to the base of the arena and
 * zero-initializes the allocated memory.
 *
 * @note	Previously written memory will be overwritten by subsequent allocs.
 *
 * @param	arena Pointer to the arena.
 */
void	arena_reset(t_arena *arena)
{
	size_t	i;
	size_t	bit;
	size_t	e;

	bit = 0;
	while (((arena->capacity >> bit) & 1) == 0)
		++bit;
	arena->base[0] = 0;
	e = 0;
	while (e < bit)
	{
		i = 1UL << e++;
		ft_memcpy(&arena->base[i], arena->base, i);
	}
	arena->head = 0;
}

/**
 * @brief	Frees the memory allocated to the arena.
 *
 * @param	arena Pointer to the arena.
 */
void	arena_destroy(t_arena *arena)
{
	if (!arena)
		return ;
	if (arena->base)
		free(arena->base);
	arena->base = NULL;
}
