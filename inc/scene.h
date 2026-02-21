#ifndef MINIRT_SCENE_H
# define MINIRT_SCENE_H

#include "defines.h"

void	validate_file_type(char *file);
void	init_scene(t_context *ctx);
void	clean_scene(t_context *ctx);
bool	hit_aabb(const t_aabb *aabb, const t_ray *ray, float closest_t, float enter);
bool	hit_bvh(uint32_t root_idx, const t_ray *ray, t_hit *hit, int32_t i, t_bvh_node *tree);
bool	hit_bvh_shadow(uint32_t root_idx, const t_ray *ray, float dist, t_bvh_node *tree);
void	init_bvh(t_context *ctx);
t_aabb	get_volume_bounds(t_object **objs, size_t n);
t_aabb	sphere_bounds(const t_object *obj);
t_aabb	cylinder_bounds(const t_object *obj);
t_aabb	cone_bounds(const t_object *obj);
t_aabb	get_object_bounds(const t_object *obj);

#endif
