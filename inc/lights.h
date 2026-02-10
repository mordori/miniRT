#ifndef MINIRT_LIGHTS_H
# define MINIRT_LIGHTS_H

# include "defines.h"

void	init_point_light(t_context *ctx, t_light *light);
bool	hit_shadow(const t_scene *scene, const t_vec3 orig, const t_vec3 dir, float dist);
void	add_lighting(const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel);

#endif
