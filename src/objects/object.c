#include "objects.h"
#include "utils.h"

static inline t_object	init_object(char *src);

void	add_object(t_context *ctx, char *src)
{
	t_object	*obj;

	obj = malloc(sizeof(*obj));
	if (!obj)
		fatal_error(ctx, "object malloc failed");
	*obj = init_object(src);
	vector_try_add(ctx, &ctx->scene.objs, obj);
}

static inline t_object	init_object(char *src)
{
	t_object	obj;

	// obj.transform.pos = ;
	// obj.type = ;
	return (obj);
}
