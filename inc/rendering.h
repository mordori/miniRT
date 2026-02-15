#ifndef MINIRT_RENDERING_H
# define MINIRT_RENDERING_H

# define _GNU_SOURCE

# include <pthread.h>
# include <unistd.h>

# include "defines.h"

bool	init_renderer(t_context *ctx);
t_vec3	trace_path(const t_context *ctx, t_pixel *pixel);
t_vec3	post_process(const t_context *ctx, const t_pixel *pixel, t_vec3 c);
t_vec3	post_process_preview(const t_context *ctx, t_vec3 c);
t_vec3	background_color(const t_texture *tex, const t_ray *ray, const float lux, t_vec2 uv_offset);
t_vec3	background_gradient(const float t);
float	reflectance(float ior);
void	specular_probability(t_path *path, t_pixel *pixel, t_vec3 fresnel);
bool	bounce_specular(t_path *path, t_vec3 f0, float ndotv);
void	bounce_diffuse(t_path *path);
t_vec3	disney_diffuse(const t_path *path, t_vec3 n, t_vec3 l, float ndotl);
void	frame_loop(void *param);
void	blit(const t_context *ctx, const t_renderer *r);
bool	rotate_skydome(t_context *ctx);

#endif
