/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:19:01 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/31 19:54:34 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	vector_try_init(t_context *ctx, t_vector *vec, bool is_shrink, void (*del)(void *))
{
	if (!vector_init(vec, is_shrink, del))
		fatal_error(ctx, errors(ERR_VECINIT), __FILE__, __LINE__);
}

bool	vector_try_add(t_context *ctx, t_vector *vec, void *item)
{
	if (!vector_add(vec, item))
		fatal_error(ctx, errors(ERR_VECADD), __FILE__, __LINE__);
	return (true);
}
