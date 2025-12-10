#ifndef MINIRT_OBJECTS_H
# define MINIRT_OBJECTS_H

# include "defines.h"

void		add_object(t_context *ctx, char **params);
t_shape		init_sphere(const t_object *obj, char **params);
bool		hit_sphere(const t_sphere *sphere, const t_ray *ray, t_hit *hit);
bool		hit_object(const t_object *obj, const t_ray *ray, t_hit *hit);

#endif
