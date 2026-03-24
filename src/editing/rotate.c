#include "editing.h"

void	obj_rotate(t_object *obj, t_vec3 axis, float angle)
{
	t_quat		rot_delta;

	rot_delta = quat_from_euler_angle(axis, angle);
	obj->transform.rot = quat_mul(rot_delta, obj->transform.rot);
	obj->transform.rot = quat_normalize(obj->transform.rot);
}
