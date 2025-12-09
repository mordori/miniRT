#ifndef MINIRT_SCENE_H
# define MINIRT_SCENE_H

#include "defines.h"

void	validate_file_type(char *file);
void	init_scene(t_context *ctx, int fd);
void	clean_scene(t_context *ctx);
void	init_skydome(t_context *ctx);
void	add_skydome_tex(t_context *ctx, char *file);
t_aabb	get_object_bounds(t_object *obj);
t_aabb	combine_boxes(t_aabb a, t_aabb b);
bool	hit_aabb(t_aabb *box, t_ray *ray);
bool	hit_bvh(t_bvh_node *node, t_ray *ray, t_hit *hit);
void	clean_bvh_recursive(t_bvh_node *node);

#endif
