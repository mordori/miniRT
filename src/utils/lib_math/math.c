#include "lib_math.h"

float ft_lerp(float a, float b, float t) {
	return (1.0f - t) * a + t * b;
}

int ft_imax(int a, int b) {
	return a > b ? a : b;
}

int ft_imin(int a, int b) {
	return a > b ? b : a;
}

size_t ft_pow(size_t n, size_t e) {
	size_t result = 1;
	size_t i = 0;
	while (i < e) {
		result *= n;
		++i;
	}
	return result;
}

bool ft_is_pot(size_t n) {
	return n != 0 && ((n & (n - 1)) == 0);
}
