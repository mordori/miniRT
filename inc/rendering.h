#ifndef RENDERING_H
#define RENDERING_H

#include <pthread.h>
#include <unistd.h>

#include "defines.h"

bool init_renderer(t_context* ctx);
t_vec3 trace_path(const t_context* ctx, t_pixel* pixel, t_render_mode mode, uint8_t bounces);
t_vec3 post_process(const t_context* ctx, const t_pixel* pixel, t_vec3 c);
t_vec3 post_process_preview(const t_context* ctx, t_vec3 c);
t_vec3 background_color(const t_scene* scene, const t_ray* ray, t_vec2 uv_offset);
float reflectance(float ior);
t_vec3 disney_brdf(const t_path* path);
void frame_loop(void* param);
void blit(const t_context* ctx, const t_renderer* r);
bool rotate_skydome(t_context* ctx);
t_vec3 bsdf(t_path* path);
t_vec3 add_lighting(const t_context* ctx, t_path* path, const t_light* light, t_pixel* pixel);
t_vec3 cook_torrance_brdf(const t_path* path);
float pdf_cos(float ndotl);
float pdf_ggx_vndf(const t_path* path);
bool sample_bsdf(t_path* path);
float power_heuristic(float pdf_d, float pdf_r);
float bsdf_pdf(t_path* path);
float light_pdf(t_vec3 l, float radius_sq);
bool config_renderer(t_context* ctx, mlx_key_data_t keydata);
void cancel_render(t_renderer* r);
void set_mode_preview(t_context* ctx, t_renderer* r, bool* update);
void set_mode_rendered(t_renderer* r);
void start_render(t_renderer* r, const t_camera* cam);
void stop_render(t_renderer* r);
void screenshot(t_context* ctx);
void resize_hook(int width, int height, void* param);
void resize_window(t_context* ctx);
t_vec2 world_to_screen(t_context* ctx, const t_camera* cam, const t_viewport* vp, t_vec3 pos);

// Sampling
// -----------------------------------------------------------------
t_vec3 sample_cos_hemisphere(t_vec3 n, t_vec2 uv);
t_vec2 sample_disk(t_vec2 uv);
t_vec3 sample_cone(t_vec3 vec, float cos_theta_max, t_vec2 uv);
void sample_ggx_vndf(t_path* path, float alpha, t_vec2 uv);

// Solid mode
// -----------------------------------------------------------------
bool trace_ray_editing(const t_context* ctx, t_path* path, t_pixel* pixel);
void add_lighting_editing(const t_context* ctx, t_path* path, const t_light* light);
void ambient_lighting(t_path* path, const t_light* light);

#endif
