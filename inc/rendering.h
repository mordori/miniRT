#ifndef MINIRT_RENDERING_H
# define MINIRT_RENDERING_H

# define _GNU_SOURCE

# include <pthread.h>
# include <unistd.h>

# include "defines.h"

bool	init_renderer(t_context *ctx);
t_vec3	trace_path(const t_context *ctx, t_pixel *pixel, t_render_mode mode, uint8_t bounces);
t_vec3	post_process(const t_context *ctx, const t_pixel *pixel, t_vec3 c);
t_vec3	post_process_preview(const t_context *ctx, t_vec3 c);
t_vec3	background_color(const t_texture *tex, const t_ray *ray, t_vec2 uv_offset);
t_vec3	background_gradient(const float t);
float	reflectance(float ior);
t_vec3	disney_brdf(const t_path *path);
void	frame_loop(void *param);
void	blit(const t_context *ctx, const t_renderer *r);
bool	rotate_skydome(t_context *ctx);
t_vec3	bsdf(t_path *path);
void	add_lighting(const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel);
t_vec3	cook_torrance_brdf(const t_path *path);
float	pdf_cos(float ndotl);
float	pdf_ggx(const t_path *path);
bool	sample_bsdf(t_path *path);
float	power_heuristic(float pdf_d, float pdf_r);
float	bsdf_pdf(t_path *path);
float	light_pdf(t_vec3 l, float radius_sq);
bool	hit_shadow(const t_scene *scene, t_vec3 orig, t_vec3 dir, float dist);


bool	trace_ray_editing(const t_context *ctx, t_path *path, t_pixel *pixel);
void	add_lighting_editing(const t_context *ctx, t_path *path, const t_light *light);
void	ambient_lighting(t_path *path, const t_light *light);

#endif
