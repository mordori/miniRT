#ifndef MINIRT_SCENE_H
# define MINIRT_SCENE_H

#include "defines.h"

t_entity	validate_file_type(char *file);
void		validate_entity(t_context *ctx, t_entity entity);
void		init_scene(t_context *ctx, int fd);
void		clean_scene(t_context *ctx);
void		add_sky_sphere_tex(t_context *ctx, t_entity entity);

#endif
