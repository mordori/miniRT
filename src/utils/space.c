#include "utils.h"

void	orthonormal_basis(const t_vec3 n, t_vec3 *t, t_vec3 *b)
{
	if (fabsf(n.x) > 0.9f)
		*t = vec3(0.0f, 1.0f, 0.0f);
	else
		*t = vec3(1.0f, 0.0f, 0.0f);
	*b = vec3_normalize(vec3_cross(n, *t));
	*t = vec3_normalize(vec3_cross(n, *b));
}
