#ifndef MINIRT_UTILS_H
# define MINIRT_UTILS_H

# define _GNU_SOURCE

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

# include "defines.h"

char	*errors(t_err_code code);
void	fatal_error(t_context *ctx, char *message, char *file, int line);
void	key_hook(mlx_key_data_t keydata, void *param);
void	resize_hook(int width, int height, void *param);
void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void* param);
void	cursor_hook(double xpos, double ypos, void* param);
void	loop_hook(void *param);
void	clean(t_context *ctx);
void	vector_try_init(\
t_context *ctx, t_vector *vec, bool is_shrink, void (*del)(void *));
void	vector_try_add(t_context *ctx, t_vector *vec, void *item);
int		try_open(char *file, int o_flag, int p_flag);
ssize_t	try_write(t_context *ctx, int fd, char *src);
ssize_t	try_write_endl(t_context *ctx, int fd, char *src);
ssize_t	try_read(t_context *ctx, int fd, char *buf, size_t n_bytes);
bool	cmp_strs(const char **types, const char *src);
int		try_gnl(t_context *ctx, int fd, char **line);
int 	cmp_bounds_x(const void *a, const void *b);
int 	cmp_bounds_y(const void *a, const void *b);
int 	cmp_bounds_z(const void *a, const void *b);
t_vec3	get_point(t_ray *ray, float t);
t_texture	load_texture(t_context *ctx, char *file);
void	free_texture(t_texture *tex);
void	resize_window(t_context *ctx);
void	tex_to_linear(t_texture *texture);
bool	resize_timer(t_context *ctx);
uint32_t	time_now(void);

#endif
