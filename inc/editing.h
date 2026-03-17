#ifndef MINIRT_EDITING_H
# define MINIRT_EDITING_H

# include "defines.h"

void	select_object(t_context *ctx);
bool	edit(t_context *ctx);
void	config_editor(t_context *ctx, mlx_key_data_t keydata);
void	apply_edit(t_context *ctx);
void	cancel_edit(t_context *ctx);

#endif
