#ifndef MINIRT_INPUT_H
# define MINIRT_INPUT_H

# include "defines.h"

void	process_input(t_context *ctx, bool *update);
int		wrap_mouse_x(const t_context *ctx, t_vec2i *pos);
int		wrap_mouse_y(const t_context *ctx, t_vec2i *pos);
bool	is_key_down(t_context *ctx);

#endif
