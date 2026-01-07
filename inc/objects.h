#ifndef MINIRT_OBJECTS_H
# define MINIRT_OBJECTS_H

# include "defines.h"
# include "parsing.h"

t_parse_error add_object(t_context *ctx, t_object *obj);
t_parse_error	init_sphere(t_context *ctx, t_vec3 center, float diameter, t_color color);
bool		hit_object(const t_object *obj, const t_ray *ray, t_hit *hit);
bool		hit_plane(const t_shape *shape, const t_ray *ray, t_hit *hit);
bool		hit_sphere(const t_shape *shape, const t_ray *ray, t_hit *hit);
bool		hit_cylinder(const t_shape *shape, const t_ray *ray, t_hit *hit);

#endif
