#include "utils.h"

t_hit	new_hit(uint32_t bounce)
{
	t_hit		hit;

	hit = (t_hit){0};
	hit.t = M_INF;
	hit.is_primary = (bounce == 0);
	return (hit);
}

void	hit_object_to_world(t_hit *hit, const t_transform *t)
{
	t_mat4		m_inv_t;
	t_vec4		p;
	t_vec4		n;

	p = vec4_3(hit->point, 1.0f);
	hit->point = mat4_mul_vec4(&t->object_to_world, p).xyz;
	m_inv_t = mat4_transpose(&t->world_to_object);
	n = vec4_3(hit->normal, 0.0f);
	hit->normal = vec3_normalize(mat4_mul_vec4(&m_inv_t, n).xyz);
}
