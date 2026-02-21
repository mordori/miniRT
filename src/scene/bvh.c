#include "scene.h"
#include "objects.h"
#include "utils.h"

static inline uint32_t	build_bvh(t_context *ctx, const t_object **objs, size_t n, t_bvh_node *tree, uint32_t *nodes);
static inline void	add_nodes_to_bvh_stack(const t_ray *ray, const t_bvh_node *node, uint32_t *stack, int32_t *i);

void	init_bvh(t_context *ctx)
{
	t_scene		*scene;
	t_object	**objs;
	size_t		n;
	uint32_t	nodes;

	scene = &ctx->scene;
	n = scene->objs.total;
	if (n == 0)
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
	objs = (t_object **)scene->objs.items;
	scene->bvh_nodes = malloc(sizeof(t_bvh_node) * ((2 * n) - 1));
	if (!scene->bvh_nodes)
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
	nodes = 0;
	scene->bvh_root_idx = build_bvh(ctx, (const t_object **)objs, n, scene->bvh_nodes, &nodes);
}

static inline uint32_t	build_bvh(t_context *ctx, const t_object **objs, size_t n, t_bvh_node *tree, uint32_t *nodes)
{
	t_bvh_node	*node;
	uint32_t	idx;
	size_t		half_n;

	idx = (*nodes)++;
	node = &tree[idx];
	*node = (t_bvh_node){0};
	if (n == 1)
	{
		node->obj = (t_object *)objs[0];
		node->aabb = get_object_bounds(node->obj);
		return (idx);
	}
	node->aabb = get_volume_bounds((t_object **)objs, n);
	sort_bvh_objects(node, objs, n);
	half_n = n / 2;
	node->left_idx = build_bvh(ctx, objs, half_n, tree, nodes);
	node->right_idx = build_bvh(ctx, objs + half_n, n - half_n, tree, nodes);
	return (idx);
}

bool	hit_bvh(uint32_t root_idx, const t_ray *ray, t_hit *hit, int32_t i, t_bvh_node *tree)
{
	uint32_t	stack[64];
	t_bvh_node	*node;
	t_hit		temp;
	bool		res;

	res = false;
	stack[i++] = root_idx;
	while (i > 0)
	{
		node = &tree[stack[--i]];
		if (!hit_aabb(&node->aabb, ray, hit->t, 0))
			continue ;
		if (node->obj)
		{
			temp.t = hit->t;
			if (!(node->obj->flags & OBJ_HIDDEN_SCENE) && !(hit->is_primary && (node->obj->flags & OBJ_HIDDEN_CAM)) && hit_object(node->obj, ray, &temp))
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

bool	hit_bvh_shadow(uint32_t root_idx, const t_ray *ray, float dist, t_bvh_node *tree)
{
	uint32_t	stack[64];
	int32_t		i;
	t_bvh_node	*node;
	t_hit		temp;

	i = 0;
	stack[i++] = root_idx;
	while (i > 0)
	{
		node = &tree[stack[--i]];
		if (!hit_aabb(&node->aabb, ray, dist, 0))
			continue ;
		if (node->obj)
		{
			temp.t = dist;
			if (!(node->obj->flags & OBJ_NO_CAST_SHADOW)
				&& hit_object(node->obj, ray, &temp))
				return (true);
			continue ;
		}
		add_nodes_to_bvh_stack(ray, node, stack, &i);
	}
	return (false);
}

static inline void	add_nodes_to_bvh_stack(const t_ray *ray, const t_bvh_node *node, uint32_t *stack, int32_t *i)
{
	if (ray->signs[node->axis])
	{
		stack[(*i)++] = node->left_idx;
		stack[(*i)++] = node->right_idx;
	}
	else
	{
		stack[(*i)++] = node->right_idx;
		stack[(*i)++] = node->left_idx;
	}
}
