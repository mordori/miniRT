#ifndef MINIRT_OBJECTS_H
# define MINIRT_OBJECTS_H

# include "defines.h"

void		add_object(t_context *ctx, char **params);
t_shape		init_sphere(char **params);
bool		hit_sphere(t_sphere *sphere, t_ray *ray, t_hit *hit);
bool		hit_object(t_object *obj, t_ray *ray, t_hit *hit);

#endif
