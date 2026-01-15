#include "utils.h"

void	sort_bvh_objects(t_bvh_node *node, const t_object **objs, size_t n)
{
	t_vec3		size;

	size = vec3_sub(node->aabb.max, node->aabb.min);
	node->axis = 0;
	if (size.y > size.x && size.y > size.z)
		node->axis = 1;
	else if (size.z > size.x && size.z > size.y)
		node->axis = 2;
	if (node->axis == 0)
		qsort(objs, n, sizeof(t_object *), cmp_bounds_x);
	else if (node->axis == 1)
		qsort(objs, n, sizeof(t_object *), cmp_bounds_y);
	else
		qsort(objs, n, sizeof(t_object *), cmp_bounds_z);
}

void	clean_bvh(t_bvh_node *node)
{
	if (!node)
		return ;
	if (node->obj)
	{
		free(node);
		return ;
	}
	clean_bvh(node->left);
	clean_bvh(node->right);
	free(node);
}
