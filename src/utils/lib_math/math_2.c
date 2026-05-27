#include "lib_math.h"

float ft_normalize(float value, float min, float max) {
	return clampf01((value - min) / (max - min));
}

float clampf01(float value) {
	return clampf(value, 0.0f, 1.0f);
}

float clampf(float value, float min, float max) {
	value = fminf(value, max);
	value = fmaxf(value, min);
	return value;
}

float clampfn11(float value) {
	return clampf(value, -1.0f, 1.0f);
}

uint32_t ft_uint_min(uint32_t a, uint32_t b) {
	return a < b ? a : b;
}
