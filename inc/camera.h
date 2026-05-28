#ifndef CAMERA_H
#define CAMERA_H

#include "defines.h"

void init_camera(t_context* ctx, t_vec3 position, t_vec3 orientation, float fov);
void update_camera(t_context* ctx, t_camera* cam);
bool control_camera(t_context* ctx, t_vec2i delta);
t_vec3 sample_defocus_disk(const t_context* ctx, t_pixel* pixel);
bool reset_camera(t_context* ctx);
bool config_camera(t_context* ctx);
void apply_cam_action(t_context* ctx, t_vec2i delta);
void begin_cam_action(t_context* ctx, t_cam_state state);
void end_cam_action(t_context* ctx);
bool is_cam_action_active(const t_context* ctx);
void apply_cam_action(t_context* ctx, t_vec2i delta);

void cam_look(t_context* ctx, t_vec2i delta);
void cam_turn(t_context* ctx, t_vec2i delta);
void cam_orbit(t_context* ctx, t_vec2i delta);
void cam_zoom(t_context* ctx, t_vec2i delta);
void cam_pan(t_context* ctx, t_vec2i delta);
bool cam_fly(t_context* ctx);

bool frame_camera(t_context* ctx, t_object* obj);
void set_default_view(t_context* ctx);

#endif
