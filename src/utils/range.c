#include "utils.h"

// Lemire's Method
uint32_t	fast_range(uint32_t n, uint32_t	range)
{
	return ((uint32_t)(((uint64_t)n * (uint64_t)range) >> 32));
}
