#include "objects.h"
#include "utils.h"

static inline t_object	init_object(char **params);

void	add_object(t_context *ctx, char **params)
{
	t_object	*obj;

	obj = malloc(sizeof(*obj));
	if (!obj)
		fatal_error(ctx, errors(ERR_OBJADD), __FILE__, __LINE__);
	*obj = init_object(params);
	vector_try_add(ctx, &ctx->scene.objs, obj);
}

static inline t_object	init_object(char **params)
{
	t_object	obj;

	(void)params;
	// obj.transform.pos = ;
	obj.type = OBJ_PLANE;
	return (obj);
}
