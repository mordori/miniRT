#ifndef MINIRT_SCENE_H
# define MINIRT_SCENE_H

#include "defines.h"

void	validate_file_type(char *file);
void	init_scene(t_context *ctx, int fd);
void	clean_scene(t_context *ctx);
void	init_sky_sphere(t_context *ctx);
void	add_sky_sphere_tex(t_context *ctx, char *file);

#endif
