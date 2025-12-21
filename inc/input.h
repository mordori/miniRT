#ifndef MINIRT_INPUT_H
# define MINIRT_INPUT_H

# include "defines.h"

bool	process_input(t_context *ctx);
int		wrap_mouse_x(const t_context *ctx, t_vec2i *pos);
int		wrap_mouse_y(const t_context *ctx, t_vec2i *pos);

#endif
