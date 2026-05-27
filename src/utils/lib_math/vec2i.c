#include "lib_math.h"

t_vec2i vec2i(int32_t x, int32_t y) {
	return (t_vec2i){ x, y };
}

t_vec2i vec2i_n(int32_t n) {
	return (t_vec2i){ n, n };
}

t_vec2i vec2i_add(t_vec2i a, t_vec2i b) {
	return (t_vec2i){ a.x + b.x, a.y + b.y };
}

t_vec2i vec2i_sub(t_vec2i a, t_vec2i b) {
	return (t_vec2i){ a.x - b.x, a.y - b.y };
}

t_vec2i vec2i_scale(t_vec2i vec, float s) {
	return (t_vec2i){ (int)((float)vec.x * s), (int)((float)vec.y * s) };
}
