/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:10:37 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 18:33:24 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_list.h"

/**
 * Adds the node `new` at the beginning of the list.
 *
 * @param lst Address of pointer to the head node of the list.
 * @param new Pointer to the node to be added.
 * @return True if successful, else false.
 */
bool	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst)
		return (false);
	if (*lst)
		new->next = *lst;
	*lst = new;
	return (true);
}
