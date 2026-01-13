#ifndef MINIRT_LIGHTS_H
# define MINIRT_LIGHTS_H

# include "defines.h"

t_vec3	compute_lighting(const t_scene *scene, const t_hit *hit, size_t idx, t_material *mat);
void	init_point_light(t_context *ctx, t_light *light);
void	init_directional_light(t_light *light);
bool	hit_shadow(const t_scene *scene, const t_hit *hit, const t_light *light, float dist);
void	compute_ambient(const t_scene *scene, t_material *mat, t_vec3 *color);

#endif
