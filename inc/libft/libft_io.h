/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_io.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:33:27 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/26 03:23:12 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_IO_H
# define LIBFT_IO_H

# include <unistd.h>

# include "libft_defs.h"

int		get_next_line(int fd, char **line);
ssize_t	ft_printf(const char *s, ...);
ssize_t	ft_putchar_fd(char c, int fd);
ssize_t	ft_putendl_fd(char *s, int fd);
ssize_t	ft_putstr_fd(char *s, int fd);
int		ft_putnbr_fd(int n, int fd);

#endif
