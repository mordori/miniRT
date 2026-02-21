#ifndef MINIRT_OBJECTS_H
# define MINIRT_OBJECTS_H

# include "defines.h"
# include "parsing.h"

t_error	add_object(t_context *ctx, t_object *obj);
bool	hit_object(const t_object *obj, const t_ray *ray, t_hit *hit);
bool	hit_plane(const t_shape *shape, const t_ray *ray, t_hit *hit);
bool	hit_sphere(const t_shape *shape, const t_ray *ray, t_hit *hit);
bool	hit_cylinder(const t_shape *shape, const t_ray *ray, t_hit *hit);
bool	hit_cone(const t_shape *shape, const t_ray *ray, t_hit *hit);
bool	hit_planes(const t_context *ctx, const t_ray *ray, t_hit *hit);

/* Object initialization */
t_error			init_plane(t_context *ctx, t_vec3 point, t_vec3 normal, uint32_t mat_id);
t_error			init_sphere(t_context *ctx, t_vec3 center, float diameter, uint32_t mat_id);
t_error			init_cylinder(t_context *ctx, t_cylinder *cy, int32_t mat_id);
t_error			init_cone(t_context *ctx, t_cone *cone, int32_t mat_id);

#endif
