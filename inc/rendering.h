#ifndef MINIRT_RENDERING_H
# define MINIRT_RENDERING_H

# define _GNU_SOURCE

# include <pthread.h>
# include <unistd.h>

# include "defines.h"

bool	init_renderer(t_context *ctx);
t_vec3	trace_path(const t_context *ctx, t_pixel *pixel);
t_vec3	post_process(const t_context *ctx, const t_pixel *pixel, t_vec3 c);
t_vec3	post_process_fast(const t_context *ctx, t_vec3 c);
// void	init_skydome(t_context *ctx, char *file);
t_vec3	background_color(const t_texture *tex, const t_ray *ray, const float lux);
t_vec3	background_gradient(const float t);

#endif
