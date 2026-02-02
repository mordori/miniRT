#include "utils.h"

int cmp_bounds_x(const void *a, const void *b)
{
	t_object	*obj_1;
	t_object	*obj_2;
	int			mask_1;
	int			mask_2;

	obj_1 = *(t_object **)a;
	obj_2 = *(t_object **)b;
	mask_1 = (obj_1->bounds_center.x < obj_2->bounds_center.x);
	mask_2 = (obj_1->bounds_center.x > obj_2->bounds_center.x);
	return (-mask_1 + mask_2);
}

int cmp_bounds_y(const void *a, const void *b)
{
	t_object	*obj_1;
	t_object	*obj_2;
	int			mask_1;
	int			mask_2;

	obj_1 = *(t_object **)a;
	obj_2 = *(t_object **)b;
	mask_1 = (obj_1->bounds_center.y < obj_2->bounds_center.y);
	mask_2 = (obj_1->bounds_center.y > obj_2->bounds_center.y);
	return (-mask_1 + mask_2);
}

int cmp_bounds_z(const void *a, const void *b)
{
	t_object	*obj_1;
	t_object	*obj_2;
	int			mask_1;
	int			mask_2;

	obj_1 = *(t_object **)a;
	obj_2 = *(t_object **)b;
	mask_1 = (obj_1->bounds_center.z < obj_2->bounds_center.z);
	mask_2 = (obj_1->bounds_center.z > obj_2->bounds_center.z);
	return (-mask_1 + mask_2);
}
