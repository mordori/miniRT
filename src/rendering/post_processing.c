#include "rendering.h"

static inline void	tonemap(t_vec3 *buffer);
static inline void	anti_alias(t_vec3 *buffer);

void	post_process(t_vec3 *buffer)
{
	anti_alias(buffer);
	tonemap(buffer);
}

static inline void	tonemap(t_vec3 *buffer)
{
	(void)buffer;
}

static inline void	anti_alias(t_vec3 *buffer)
{
	(void)buffer;
}
