#ifndef UI_HPP
#define UI_HPP

#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

bool check_ui_dirty(void);

void init_ui(void);
void render_ui(void* param);
void cleanup_ui(void);

#ifdef __cplusplus
}
#endif

#endif
