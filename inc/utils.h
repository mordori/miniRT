#ifndef MINIRT_UTILS_H
# define MINIRT_UTILS_H

# define _GNU_SOURCE

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

# include "defines.h"

char	*errors(t_err_code code);
void	fatal_error(t_context *ctx, char *message, char *file, int line);
void	key_hook(mlx_key_data_t keydata, void *param);
void	resize_hook(int width, int height, void *param);
void	update_hook(void *param);
void	clean(t_context *ctx);
void	vector_try_add(t_context *ctx, t_vector *vec, void *item);
int		try_open(char *file, int o_flag, int p_flag);
ssize_t	try_write(t_context *ctx, int fd, char *src);
ssize_t	try_write_endl(t_context *ctx, int fd, char *src);
ssize_t	try_read(t_context *ctx, int fd, char *buf, size_t n_bytes);
bool	cmp_strs(const char **types, const char *src);
int		try_gnl(t_context *ctx, int fd, char **line);

#endif
