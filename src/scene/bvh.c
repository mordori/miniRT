#include "scene.h"
#include "objects.h"
#include "utils.h"

t_bvh_node	*build_bvh(t_context *ctx, const t_object **objs, size_t n);
static inline void	sort_objects(t_bvh_node *node, const t_object **objs, size_t n);

void	init_bvh(t_context *ctx)
{
	t_scene		*scene;
	t_object	**objs;
	size_t		n;

	scene = &ctx->scene;
	n = scene->objs.total;
	objs = (t_object **)scene->objs.items;
	scene->bvh_root = build_bvh(ctx, (const t_object **)objs, n);
}

t_bvh_node	*build_bvh(t_context *ctx, const t_object **objs, size_t n)
{
	t_bvh_node	*node;
	size_t		half_n;

	node = malloc(sizeof(*node));
	if (!node)
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
	*node = (t_bvh_node){0};
	if (n == 1)
	{
		node->obj = (t_object *)objs[0];
		return (node);
	}
	node->aabb = get_volume_bounds((t_object **)objs, n);
	sort_objects(node, objs, n);
	half_n = n / 2;
	node->left = build_bvh(ctx, objs, half_n);
	node->right = build_bvh(ctx, objs + half_n, n - half_n);
	return (node);
}

static inline void	sort_objects(t_bvh_node *node, const t_object **objs, size_t n)
{
	t_vec3	size;

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

bool	hit_bvh(t_bvh_node *node, const t_ray *ray, t_hit *hit)
{
	bool	left;
	bool	right;

	if (!hit_aabb(&node->aabb, ray, hit->t))
		return (false);
	if (!node->left && !node->right)
		return (hit_object(node->obj, ray, hit));
	if (ray->sign[node->axis])
	{
		right = hit_bvh(node->right, ray, hit);
		left = hit_bvh(node->left, ray, hit);
		return (right || left);
	}
	else
	{
		left = hit_bvh(node->left, ray, hit);
		right = hit_bvh(node->right, ray, hit);
		return (left || right);
	}
}

bool	hit_bvh_shadow(t_bvh_node *node, const t_ray *ray, t_hit *hit)
{
	bool	left;
	bool	right;

	if (!hit_aabb(&node->aabb, ray, hit->t))
		return (false);
	if (!node->left && !node->right)
		return (hit_object(node->obj, ray, hit));
	if (ray->sign[node->axis])
	{
		right = hit_bvh(node->right, ray, hit);
		left = hit_bvh(node->left, ray, hit);
		return (right || left);
	}
	else
	{
		left = hit_bvh(node->left, ray, hit);
		right = hit_bvh(node->right, ray, hit);
		return (left || right);
	}
}
