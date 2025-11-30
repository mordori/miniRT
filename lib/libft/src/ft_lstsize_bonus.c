/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:22:09 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 18:44:56 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_list.h"

/**
 * Counts the number of nodes in `list`.
 *
 * @param lst Head node of the list.
 * @return Length of `list`.
 */
int	ft_lstsize(t_list *lst)
{
	int	len;

	if (!lst)
		return (false);
	len = 0;
	while (lst)
	{
		++len;
		lst = lst->next;
	}
	return (len);
}
