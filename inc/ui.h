#ifndef UI_H
#define UI_H

#include "defines.h"

void update_ui(t_context* ctx);
void set_text_color(mlx_image_t* img, uint32_t color);
t_ui ui_val(int32_t* last, int32_t current);
t_ui ui_valf(float* lastf, float currentf);

#endif
