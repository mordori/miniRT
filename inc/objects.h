#ifndef MINIRT_OBJECTS_H
# define MINIRT_OBJECTS_H

# include "defines.h"
# include "parsing.h"

t_error	add_object(t_context *ctx, t_object *obj);
bool	hit_object(const t_object *obj, const t_ray *ray, t_hit *hit);
bool	hit_plane(const t_shape *shape, const t_ray *ray, t_hit *hit);
bool	hit_sphere(const t_shape *shape, const t_ray *ray, t_hit *hit);
bool	hit_cylinder(const t_shape *shape, const t_ray *ray, t_hit *hit);
t_vec3	random_point_on_object(const t_object *obj, float u, float v);
t_vec3	random_point_on_sphere(const t_shape *shape, float u, float v);
t_vec3	random_point_on_plane(const t_shape *shape, float u, float v);
t_vec3	random_point_on_cylinder(const t_shape *shape, float u, float v);
t_vec3	normal_at_point(const t_object *obj, const t_vec3 pos);
t_vec3	normal_at_plane(const t_shape *shape, const t_vec3 pos);
t_vec3	normal_at_sphere(const t_shape *shape, const t_vec3 pos);
t_vec3	normal_at_cylinder(const t_shape *shape, const t_vec3 pos);

#endif
