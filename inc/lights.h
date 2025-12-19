#ifndef MINIRT_LIGHTS_H
# define MINIRT_LIGHTS_H

# include "defines.h"

void	add_light(t_context *ctx, char **params);
t_vec4	calculate_lighting(const t_scene *scene, const t_hit *hit);
void	init_point_light(t_context *ctx, t_light light);
bool	in_shadow(const t_scene *scene, const t_hit *surf, t_light *light);

#endif
