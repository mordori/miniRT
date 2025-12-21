#ifndef MINIRT_LIGHTS_H
# define MINIRT_LIGHTS_H

# include "defines.h"

void	add_light(t_context *ctx, char **params);
t_vec4	calculate_lighting(const t_scene *scene, const t_hit *hit, size_t idx, float ndotl);
void	init_point_light(t_context *ctx, const t_light *light);
bool	hit_shadow(const t_scene *scene, const t_hit *hit, const t_light *light, float dist);

#endif
