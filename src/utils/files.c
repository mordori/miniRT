#include "utils.h"
#include "libft_str.h"
#include "libft_io.h"

ssize_t	try_write(t_context *ctx, int fd, char *src)
{
	ssize_t		bytes;
	ssize_t		len;
	size_t		ulen;

	ulen = ft_strlen(src);
	if (ulen > SSIZE_MAX)
		fatal_error(ctx, errors(ERR_SSIZE), __FILE__, __LINE__);
	len = (ssize_t)ulen;
	bytes = write(fd, src, len);
	if (bytes != len)
	{
		close(fd);
		fatal_error(ctx, errors(ERR_WRITE), __FILE__, __LINE__);
	}
	return (bytes);
}

ssize_t	try_write_endl(t_context *ctx, int fd, char *src)
{
	ssize_t		bytes;

	bytes = try_write(ctx, fd, src);
	bytes += try_write(ctx, fd, "\n");
	return (bytes);
}

int	try_open(char *file, int o_flag, int p_flag)
{
	int		fd;

	fd = open(file, o_flag, p_flag);
	if (fd == ERROR)
		fatal_error(NULL, errors(ERR_OPEN), __FILE__, __LINE__);
	return (fd);
}

ssize_t	try_read(t_context *ctx, int fd, char *buf, size_t n_bytes)
{
	ssize_t		bytes;

	bytes = read(fd, buf, n_bytes);
	if (bytes == ERROR)
	{
		close(fd);
		fatal_error(ctx, errors(ERR_READ), __FILE__, __LINE__);
	}
	return (bytes);
}

int	try_gnl(t_context *ctx, int fd, char **line)
{
	int		status;

	status = get_next_line(fd, line);
	if (status == GNL_ERROR)
		fatal_error(ctx, errors(ERR_GNL), __FILE__, __LINE__);
	return (status);
}
