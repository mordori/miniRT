#include "editing.h"

void	obj_translate(t_object *obj, t_vec3 axis_delta)
{
	obj->transform.pos = vec3_add(obj->transform.pos, axis_delta);
}
