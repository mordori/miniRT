/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:48:12 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 01:00:06 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "libft_str.h"

ssize_t	try_write(t_context	*ctx, int fd, char *src)
{
	ssize_t	bytes;
	ssize_t	len;
	size_t	ulen;

	ulen = ft_strlen(src);
	if (ulen > SSIZE_MAX)
		fatal_error(ctx, "write: src is longer than SSIZE_MAX");
	len = (ssize_t)ulen;
	bytes = write(fd, src, len);
	if (bytes != len)
	{
		close(fd);
		fatal_error(ctx, "write failed");
	}
	return (bytes);
}

ssize_t	try_write_endl(t_context	*ctx, int fd, char *src)
{
	ssize_t	bytes;

	bytes = try_write(ctx, fd, src);
	bytes += try_write(ctx, fd, "\n");
	return (bytes);
}

int	try_open(t_context	*ctx, char *file, int o_flag, int p_flag)
{
	int	fd;

	fd = open(file, o_flag, p_flag);
	if (fd == ERROR)
	{
		warning(ctx, file);
	}
	return (fd);
}

ssize_t	try_read(t_context	*ctx, int fd, char *buf, size_t n_bytes)
{
	ssize_t	bytes;

	bytes = read(fd, buf, n_bytes);
	if (bytes == ERROR)
	{
		close(fd);
		fatal_error(ctx, "read failed");
	}
	return (bytes);
}
