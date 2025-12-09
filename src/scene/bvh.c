#include "scene.h"
#include "objects.h"
#include "utils.h"

static inline t_bvh_node	*bvh_recursive(t_context *ctx, t_vector *objs, size_t n);

void	init_bvh(t_context *ctx)
{
	t_scene	*scene;

	scene = &ctx->scene;
	scene->bvh_root = bvh_recursive(ctx, &scene->objs, vector_total(&scene->objs));
}

static inline t_bvh_node	*bvh_recursive(t_context *ctx, t_vector *objs, size_t n)
{
	t_bvh_node	*node;

	node = malloc(sizeof(*node));
	if (!node)
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
}

bool	hit_bvh(t_bvh_node *node, t_ray *ray, t_hit *hit)
{
	bool	left;
	bool	right;

	if (!hit_aabb(&node->box, ray))
		return (false);
	if (!node->left && !node->right)
		return (hit_object(node->obj, ray, hit));
	left = hit_bvh(node->left, ray, hit);
	right = hit_bvh(node->right, ray, hit);
	return (left || right);
}

void	clean_bvh_recursive(t_bvh_node *node)
{
	if (node->obj)
	{
		free(node);
		node = NULL;
		return ;
	}
	clean_bvh_recursive(node->left);
	clean_bvh_recursive(node->right);
	free(node);
	node = NULL;
}
