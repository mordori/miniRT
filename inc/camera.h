#ifndef MINIRT_CAMERA_H
# define MINIRT_CAMERA_H

# include "defines.h"

void	add_camera(t_context *ctx, char **params);
void	update_camera(t_context *ctx);
bool	control_camera(t_context *ctx);

#endif
