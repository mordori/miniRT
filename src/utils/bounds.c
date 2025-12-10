#include "utils.h"

int cmp_bounds_x(const void *a, const void *b)
{
	t_object	*obj_1;
	t_object	*obj_2;

	obj_1 = *(t_object **)a;
	obj_2 = *(t_object **)b;
	if (obj_1->bounds_center.x < obj_2->bounds_center.x)
		return (-1);
	if (obj_1->bounds_center.x > obj_2->bounds_center.x)
		return (1);
	return (0);
}

int cmp_bounds_y(const void *a, const void *b)
{
	t_object	*obj_1;
	t_object	*obj_2;

	obj_1 = *(t_object **)a;
	obj_2 = *(t_object **)b;
	if (obj_1->bounds_center.y < obj_2->bounds_center.y)
		return (-1);
	if (obj_1->bounds_center.y > obj_2->bounds_center.y)
		return (1);
	return (0);
}

int cmp_bounds_z(const void *a, const void *b)
{
	t_object	*obj_1;
	t_object	*obj_2;

	obj_1 = *(t_object **)a;
	obj_2 = *(t_object **)b;
	if (obj_1->bounds_center.z < obj_2->bounds_center.z)
		return (-1);
	if (obj_1->bounds_center.z > obj_2->bounds_center.z)
		return (1);
	return (0);
}
