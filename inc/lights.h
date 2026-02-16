#ifndef MINIRT_LIGHTS_H
# define MINIRT_LIGHTS_H

# include "defines.h"

void	init_point_light(t_context *ctx, t_light *light);
void	init_directional_light(t_context *ctx, t_light *light);
bool	hit_shadow(const t_scene *scene, const t_vec3 orig, const t_vec3 dir, float dist);
t_vec3	compute_ambient(const t_scene *scene, const t_material *mat, const t_hit *hit);
t_vec3	compute_directional(const t_scene *scene, const t_hit *hit, const t_material *mat);

#endif
