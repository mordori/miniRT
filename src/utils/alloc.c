/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:21:05 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 19:21:07 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	*a_alloc(size_t alignment, size_t size)
{
	size_t	max;

	max = alignment - 1;
	size = (size + max) & ~max;
	return (aligned_alloc(alignment, size));
}
