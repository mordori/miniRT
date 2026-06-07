#ifndef UI_HPP
#define UI_HPP

#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

bool ui_check_dirty(void);
bool ui_want_mouse(void);
bool ui_want_keyboard(void);

void init_ui(void);
void render_ui(void* param);
void cleanup_ui(void);

#ifdef __cplusplus
}
#endif

#endif
