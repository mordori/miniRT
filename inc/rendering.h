#ifndef MINIRT_RENDERING_H
# define MINIRT_RENDERING_H

# define _GNU_SOURCE

# include <pthread.h>
# include <unistd.h>

# include "defines.h"

bool	init_renderer(t_context *ctx);
t_vec4	trace_ray(const t_scene *scene, const uint32_t x, const uint32_t y);
void	post_process(t_vec3 *buffer);
bool	render(t_renderer *r);

#endif
