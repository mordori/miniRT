#include "utils.h"

void* a_alloc(size_t alignment, size_t size) {
	size_t max = alignment - 1;
	size = (size + max) & ~max;
	return aligned_alloc(alignment, size);
}

void vector_try_init(t_context* ctx, t_vector* vec, bool is_shrink, void (*del)(void*)) {
	if (!vector_init(vec, is_shrink, del))
		fatal_error(ctx, errors(ERR_VECINIT), __FILE__, __LINE__);
}

bool vector_try_add(t_context* ctx, t_vector* vec, void* item) {
	if (!vector_add(vec, item))
		fatal_error(ctx, errors(ERR_VECADD), __FILE__, __LINE__);
	return true;
}
