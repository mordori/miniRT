#ifndef MINIRT_RENDERING_H
# define MINIRT_RENDERING_H

# define _GNU_SOURCE

# include <pthread.h>
# include <unistd.h>

# include "defines.h"

t_ray	new_ray(t_vec3 origin, t_vec3 dir);
bool	init_renderer(t_context *ctx);
t_vec3	trace_path(const t_context *ctx, t_pixel *pixel);
t_vec3	post_process(const t_context *ctx, const t_pixel *pixel, t_vec3 c);
t_vec3	post_process_fast(t_vec3 c, float exposure);

#endif
