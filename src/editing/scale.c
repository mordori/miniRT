#include "editing.h"

void	obj_scale(t_object *obj, t_vec3 axis_delta)
{
	obj->transform.scale = vec3_mul(obj->transform.scale, axis_delta);
	obj->transform.scale.v = _mm_max_ps(obj->transform.scale.v, vec4_3(vec3_n(0.01f), 1.0f).v);
}
