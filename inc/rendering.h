#ifndef MINIRT_RENDERING_H
# define MINIRT_RENDERING_H

# define _GNU_SOURCE

# include <pthread.h>
# include <unistd.h>

# include "defines.h"

t_ray	new_ray(t_vec3 origin, t_vec3 dest);
bool	init_renderer(t_context *ctx);
t_vec4	trace_ray(const t_scene *scene, uint32_t x, uint32_t y);
t_vec3	post_process(t_vec3 c);
bool	start_render(t_renderer *r);
void	blit(t_image *img, t_renderer *r);

#endif
