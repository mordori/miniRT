#include "utils.h"

void	vector_try_add(t_context *ctx, t_vector *vec, void *item)
{
	if (!vector_add(vec, item))
		fatal_error(ctx, "vector add failed");
}
