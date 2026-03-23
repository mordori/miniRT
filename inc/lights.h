#ifndef LIGHTS_H
# define LIGHTS_H

# include "defines.h"
# include "parsing.h"

t_error	init_point_light(t_context *ctx, t_light *light, uint32_t mat_id);

#endif
