#ifndef EDITING_H
#define EDITING_H

#include "defines.h"

void select_object(t_context* ctx);
bool edit_object(t_context* ctx, t_vec2i delta);
bool config_editor(t_context* ctx, mlx_key_data_t keydata);
void apply_edit_action(t_context* ctx);
bool cancel_edit_action(t_context* ctx);
void begin_edit_action(t_context* ctx, t_edit_mode mode);
void end_edit_action(t_context* ctx);
void edit_action(t_context* ctx, t_vec2i delta);
void obj_translate(t_context* ctx, float magnitude, t_vec2i delta, float speed);
void obj_rotate(t_context* ctx, float angle);
void obj_scale(t_context* ctx, float magnitude);
void set_axis_constraints(t_context* ctx, mlx_key_data_t keydata);
float eval_scale_magnitude(t_context* ctx, t_vec2i delta, float speed);
float eval_speed(t_context* ctx);
float eval_magnitude(t_context* ctx, t_vec2i delta, float speed);
bool deselect_object(t_context* ctx, t_renderer* r);
void reset_editor(t_context* ctx, t_renderer* r);

#endif
