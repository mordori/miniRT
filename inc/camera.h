#ifndef CAMERA_H
# define CAMERA_H

# include "defines.h"

void	init_camera(t_context *ctx, t_vec3 position, t_vec3 orientation,
			float fov);
void	update_camera(t_context *ctx, t_camera *cam);
bool	control_camera(t_context *ctx);
bool	camera_movement(t_context *ctx);
t_vec3	sample_defocus_disk(const t_context *ctx, t_pixel *pixel);
void	reset_camera(t_context *ctx);
bool	config_camera(t_context *ctx);

void	apply_cam_action(t_context *ctx, t_vec2i delta);

#endif
