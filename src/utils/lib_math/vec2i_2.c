#include "lib_math.h"

t_vec2i vec2i_f(float x, float y) {
	return (t_vec2i){ (int32_t)x, (int32_t)y };
}

t_vec2i vec2i_nf(float n) {
	return (t_vec2i){ (int32_t)n, (int32_t)n };
}
