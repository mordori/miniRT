/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:58:59 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 18:08:02 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_list.h"

/**
 * Deletes and frees the given node and all its
 * successors, using the functions `del` and free().
 *
 * Finally, set the pointer to the list to NULL.
 *
 * @param lst Pointer to head node of a list.
 * @param del Pointer to a function to delete `content` of a node.
 * @return True if successful, else false.
 */
bool	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!lst || !*lst || !del)
		return (false);
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		(*lst) = temp;
	}
	return (true);
}
