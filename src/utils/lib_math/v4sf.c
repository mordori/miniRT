#include "lib_math.h"

t_v4sf v4sf(float x, float y, float z, float w) {
	return (t_v4sf){ x, y, z, w };
}

t_v4sf v4sf_n(float n) {
	return (t_v4sf){ n, n, n, n };
}
