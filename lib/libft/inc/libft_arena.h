/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_arena.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 20:47:14 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 00:14:54 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_ARENA_H
# define LIBFT_ARENA_H

# include <stdlib.h>

# include "libft_defs.h"

struct s_arena
{
	char	*base;
	size_t	capacity;
	size_t	head;
};

void	*arena_alloc(t_arena *arena, size_t size);
void	arena_destroy(t_arena *arena);
void	arena_reset(t_arena *arena);
t_arena	arena_create(\
void *ctx, size_t capacity, void (*e)(void *ctx, char *msg));

#endif
