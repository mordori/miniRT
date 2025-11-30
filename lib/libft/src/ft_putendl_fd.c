/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:07:01 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 18:39:07 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_io.h"

/**
 * Outputs string `s` to the specified file descriptor followed by a newline.
 *
 * @param s Source string.
 * @param fd File descriptor.
 * @return Number of characters written, ERROR on error or only partial write.
 */
ssize_t	ft_putendl_fd(char *s, int fd)
{
	ssize_t	bytes;

	if (!s)
		return (ERROR);
	bytes = ft_putstr_fd(s, fd);
	if (bytes == ERROR || write (fd, "\n", 1) == ERROR)
		return (ERROR);
	return (bytes + 1);
}
