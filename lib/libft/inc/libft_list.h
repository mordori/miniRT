/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:43:45 by myli-pen          #+#    #+#             */
/*   Updated: 2025/11/30 20:08:36 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_LIST_H
# define LIBFT_LIST_H

# include "libft_defs.h"

struct s_list
{
	void			*content;
	struct s_list	*next;
};

int		ft_lstsize(t_list *lst);
bool	ft_lstadd_front(t_list **lst, t_list *new);
bool	ft_lstadd_back(t_list **lst, t_list *new);
bool	ft_lstdelone(t_list *lst, void (*del)(void *));
bool	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstnew(void *content);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif
