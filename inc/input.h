#ifndef MINIRT_INPUT_H
# define MINIRT_INPUT_H

# include "defines.h"

void	process_input(t_context *ctx, bool *update);
t_vec2i	get_mouse_delta(t_context *ctx);

#endif
