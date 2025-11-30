/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:00:56 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 16:07:05 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_list.h"

/**
 * Iterater through `lst` and applies function `f´ to each node's `content`,
 * and creates a new list resulting of the successive applications of `f`.
 *
 * Function `del` is used to delete `content` of the nodes if needed.
 *
 * @param lst Head node of the list.
 * @param f Pointer to function applied to each node's `content`.
 * @param del Pointer to a function to delete `content` of a node.
 * @return New list, or NULL if allocation fails.
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*temp;
	t_list	*lst_new;

	if (!lst || !f || !del)
		return (NULL);
	temp = NULL;
	lst_new = NULL;
	while (lst)
	{
		temp = ft_lstnew(f(lst->content));
		if (!temp)
		{
			ft_lstclear(&lst_new, del);
			return (NULL);
		}
		ft_lstadd_back(&lst_new, temp);
		lst = lst->next;
	}
	return (lst_new);
}
