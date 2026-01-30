#include "utils.h"

void	*a_alloc(size_t alignment, size_t size)
{
	size_t	max;

	max = alignment - 1;
	size = (size + max) & ~max;
	return (aligned_alloc(alignment, size));
}
