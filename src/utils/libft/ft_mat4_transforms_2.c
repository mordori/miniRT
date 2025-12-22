#include "libft_matrix.h"

/**
 * Creates a translation matrix that translates by vector t (x, y, z).
 *
 * [ 1		xy		xz		0 ]
 * [ yx		1		yz		0 ]
 * [ zx		zy		1		0 ]
 * [ 0		0		0		1 ]
 *
 * @param s Vec3 position vector.
 * @return 4x4 matrix product.
 */
t_mat4	mat4_shear(t_vec3 x, t_vec3 y, t_vec3 z)
{
	t_mat4	matrix;

	matrix = mat4_identity();
	matrix.m[0][1] = x.y;
	matrix.m[0][2] = x.z;
	matrix.m[1][0] = y.x;
	matrix.m[1][2] = y.z;
	matrix.m[2][0] = z.x;
	matrix.m[2][1] = z.y;
	return (matrix);
}
