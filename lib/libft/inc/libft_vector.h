/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_vector.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:30:08 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 22:22:31 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_VECTOR_H
# define LIBFT_VECTOR_H

# include <stdlib.h>
# include <stdarg.h>

# include "libft_defs.h"

typedef struct s_vector
{
	void	**items;
	size_t	size;
	size_t	total;
	bool	is_shrink;
	void	(*del)(void *);
}				t_vector;

bool	vector_add(t_vector *vec, void *new);
bool	vector_free(t_vector *vec, ...);
bool	vector_del(t_vector *vec, size_t index);
void	*vector_get(t_vector *vec, size_t index);
bool	vector_init(t_vector *vec, bool is_shrink, void (*del)(void *));
bool	vector_set(t_vector *vec, size_t index, void *new);
size_t	vector_size(t_vector *vec);
size_t	vector_total(t_vector *vec);
void	*vector_getlast(t_vector *vec);
bool	vector_insert(t_vector *vec, void *new, size_t index);
void	vector_clean_items(t_vector *vec, void (*del)(void *));

#endif
