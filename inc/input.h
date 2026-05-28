#ifndef INPUT_H
#define INPUT_H

#include "defines.h"

void process_input(t_context* ctx, bool* update);
t_vec2i get_mouse_delta(t_context* ctx);
void key_hook(mlx_key_data_t keydata, void* param);
void mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void* param);

#endif
