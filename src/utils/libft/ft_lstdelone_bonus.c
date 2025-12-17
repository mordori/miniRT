/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:57:30 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 18:05:58 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft_list.h"

/**
 * Deletes `content` from `lst` with function `del`, and then free() the node.
 *
 * @param lst Pointer to a node.
 * @param del Pointer to function used to delete `content` of the node.
 * @return True if successful, else false.
 */
bool	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return (false);
	del (lst->content);
	free(lst);
	return (true);
}
