#include "utils.h"

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
