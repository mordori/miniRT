/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 20:59:54 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/09 13:24:49 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_io.h"
#include "libft_str.h"

/**
 * Outputs string `s` to the specified file descriptor.
 *
 * @param s Source string.
 * @param fd File descriptor.
 * @return Number of characters written, ERROR on error or only partial write.
 */
ssize_t	ft_putstr_fd(char *s, int fd)
{
	ssize_t	bytes;
	ssize_t	totalbytes;
	size_t	len;

	if (!s)
		return (ERROR);
	totalbytes = 0;
	len = ft_strlen(s);
	while (len > 0)
	{
		bytes = write (fd, s, len);
		if (bytes < 0)
			return (ERROR);
		totalbytes += bytes;
		len -= bytes;
	}
	return (totalbytes);
}
