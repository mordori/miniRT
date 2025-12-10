#ifndef MINIRT_RENDERING_H
# define MINIRT_RENDERING_H

# define _GNU_SOURCE

# include <pthread.h>
# include <unistd.h>

# include "defines.h"

bool	init_renderer(t_context *ctx);
void	get_render_job(t_context *ctx);
t_vec4	trace(const t_scene *scene, int x, int y);
void	post_process(float *buffer);

#endif
