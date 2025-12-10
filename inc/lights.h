#ifndef MINIRT_LIGHTS_H
# define MINIRT_LIGHTS_H

# include "defines.h"

void	add_light(t_context *ctx, char **params);
t_vec4	calculate_lighting(const t_scene *scene, const t_hit *hit);

#endif
