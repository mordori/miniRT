#include "scene.h"
#include "objects.h"
#include "utils.h"

static inline t_bvh_node	*build_bvh(t_context *ctx, const t_object **objs, size_t n);
static inline void	add_nodes_to_bvh_stack(const t_ray *ray, const t_bvh_node *node, t_bvh_node **stack, int32_t *i);

void	init_bvh(t_context *ctx)
{
	t_scene		*scene;
	t_object	**objs;
	size_t		n;

	scene = &ctx->scene;
	n = scene->objs.total;
	if (n == 0)
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
	objs = (t_object **)scene->objs.items;
	scene->bvh_root = build_bvh(ctx, (const t_object **)objs, n);
}

// TODO: Linear BVH with Morton codes
static inline t_bvh_node	*build_bvh(t_context *ctx, const t_object **objs, size_t n)
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
		node->aabb = get_object_bounds(node->obj);
		return (node);
	}
	node->aabb = get_volume_bounds((t_object **)objs, n);
	sort_bvh_objects(node, objs, n);
	half_n = n / 2;
	node->left = build_bvh(ctx, objs, half_n);
	node->right = build_bvh(ctx, objs + half_n, n - half_n);
	return (node);
}

bool	hit_bvh(t_bvh_node *root, const t_ray *ray, t_hit *hit, int32_t i)
{
	t_bvh_node	*stack[64];
	t_bvh_node	*node;
	t_hit		temp;
	bool		res;

	res = false;
	stack[i++] = root;
	while (i > 0)
	{
		node = stack[--i];
		if (!hit_aabb(&node->aabb, ray, hit->t, 0))
			continue ;
		if (node->obj)
		{
			temp.t = hit->t;
			if ((node->obj->flags & OBJ_VISIBLE) && !(hit->is_primary && (node->obj->flags & OBJ_HIDDEN_CAM)) && hit_object(node->obj, ray, &temp))
			{
				res = true;
				*hit = temp;
			}
			continue ;
		}
		add_nodes_to_bvh_stack(ray, node, stack, &i);
	}
	return (res);
}

bool	hit_bvh_shadow(t_bvh_node *root, const t_ray *ray, float dist)
{
	t_bvh_node	*stack[64];
	int32_t		i;
	t_bvh_node	*node;
	t_hit		temp;

	i = 0;
	stack[i++] = root;
	while (i > 0)
	{
		node = stack[--i];
		if (!hit_aabb(&node->aabb, ray, dist, 0))
			continue ;
		if (node->obj)
		{
			temp.t = dist;
			if (!(node->obj->flags & OBJ_NO_CAST_SHADOW) && hit_object(node->obj, ray, &temp))
				return (true);
			continue ;
		}
		add_nodes_to_bvh_stack(ray, node, stack, &i);
	}
	return (false);
}

static inline void	add_nodes_to_bvh_stack(const t_ray *ray, const t_bvh_node *node, t_bvh_node **stack, int32_t *i)
{
	if (ray->signs[node->axis])
	{
		stack[(*i)++] = node->left;
		stack[(*i)++] = node->right;
	}
	else
	{
		stack[(*i)++] = node->right;
		stack[(*i)++] = node->left;
	}
}
