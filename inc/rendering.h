#ifndef MINIRT_RENDERING_H
# define MINIRT_RENDERING_H

# define _GNU_SOURCE

# include <pthread.h>
# include <unistd.h>

# include "defines.h"

bool	init_renderer(t_context *ctx);
void	render();
void	get_render_job(t_context *ctx);

#endif
