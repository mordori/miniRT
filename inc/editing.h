#ifndef MINIRT_EDITING_H
# define MINIRT_EDITING_H

# include "defines.h"

void	select_object(t_context *ctx);
bool	edit_object(t_context *ctx, t_vec2i delta);
void	config_editor(t_context *ctx, mlx_key_data_t keydata);
void	apply_edit_action(t_context *ctx);
void	cancel_edit_action(t_context *ctx);
void	obj_translate(t_context *ctx, t_vec3 delta);
void	obj_rotate(t_context *ctx, t_vec3 delta);
void	obj_scale(t_context *ctx, t_vec3 delta);
void	set_axis_constraints(t_context *ctx, mlx_key_data_t keydata);

#endif
