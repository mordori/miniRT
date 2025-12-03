#include "utils.h"
#include "libft_str.h"
#include "libft_io.h"

ssize_t	try_write(t_context *ctx, int fd, char *src)
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

ssize_t	try_write_endl(t_context *ctx, int fd, char *src)
{
	ssize_t	bytes;

	bytes = try_write(ctx, fd, src);
	bytes += try_write(ctx, fd, "\n");
	return (bytes);
}

int	try_open(char *file, int o_flag, int p_flag)
{
	int	fd;

	fd = open(file, o_flag, p_flag);
	if (fd == ERROR)
		fatal_error(NULL, "open failed");
	return (fd);
}

ssize_t	try_read(t_context *ctx, int fd, char *buf, size_t n_bytes)
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

int	try_gnl(t_context *ctx, int fd, char **line)
{
	int	status;

	status = get_next_line(fd, line);
	if (status == GNL_ERROR)
		fatal_error(ctx, "GNL failed");
	return (status);
}
