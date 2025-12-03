#ifndef MINIRT_RENDERING_H
# define MINIRT_RENDERING_H

# define _GNU_SOURCE

# include <pthread.h>
# include <unistd.h>

# include "defines.h"

bool	init_threads(t_context *ctx);

void	render();
void	get_render_bucket();
void	update_viewport();

#endif
