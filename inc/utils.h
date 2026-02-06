#ifndef MINIRT_UTILS_H
# define MINIRT_UTILS_H

# define _GNU_SOURCE

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
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
t_vec3	get_point(const t_ray *ray, const float t);
t_texture	load_texture(t_context *ctx, char *file, bool is_srgb);
void	free_texture(t_texture *tex);
void	resize_window(t_context *ctx);
bool	resize_timer(const t_context *ctx);
uint32_t	time_now(void);
void	clean_bvh(t_bvh_node *node);
void	start_render(t_renderer *r, const t_camera *cam);
void	stop_render(t_renderer *r);
float	ft_atof(const char *str, char **endptr);
double	ft_strtod(const char *str, char **endptr);
char	**try_split(char const *s);
void	blit(const t_context *ctx, const t_renderer *r, uint32_t i, uint32_t limit);
void	sort_bvh_objects(t_bvh_node *node, const t_object **objs, size_t n);
float	blue_noise(const t_texture *tex, const t_pixel *pixel, const uint32_t dim);
t_vec3	map_spherical(const float u, const float v);
t_vec2	spherical_uv(t_vec3 dir);
uint32_t	fast_range(const uint32_t n, const uint32_t range);
size_t	uint64_to_str(uint64_t n, char *buf);
void	*a_alloc(size_t alignment, size_t size);
t_ray	new_ray(const t_vec3 origin, const t_vec3 dir);

#endif
