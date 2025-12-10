#include "rendering.h"

static inline void	tonemap(float *buffer);
static inline void	anti_alias(float *buffer);

void	post_process(float *buffer)
{
	anti_alias(buffer);
	tonemap(buffer);
}

static inline void	tonemap(float *buffer)
{
	(void)buffer;
}

static inline void	anti_alias(float *buffer)
{
	(void)buffer;
}
