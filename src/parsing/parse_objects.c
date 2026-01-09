#include "parsing.h"
#include "objects.h"
#include "libft_str.h"
#include "libft_vector.h"
#include "utils.h"
#include <stdlib.h>

// t_parse_error add_object_to_scene(t_context *ctx, t_object *obj)
// {
// 	t_object *new_obj;

// 	new_obj = malloc(sizeof(t_object));
// 	if (!new_obj)
// 		return (PARSE_ERR_MEMORY);
// 	*new_obj = *obj;
// 	// *obj = init_object(obj);
// 	vector_try_add(ctx, &ctx->scene.objs, new_obj);
// 	return (PARSE_OK);
// }
// Sphere:
// sp 0.0,0.0,20.6 12.6 10,0,255
// ∗ identifier: sp
// ∗ x, y, z coordinates of the sphere center: 0.0,0.0,20.6
// ∗ the sphere diameter: 12.6
// ∗ R,G,B colors in the range [0-255]: 10, 0, 255
t_parse_error parse_sphere(t_context *ctx, char **tokens)
{
	t_vec3	center;
	float	diameter;
	t_color	color;

	if (count_tokens(tokens) != 4)
		return (PARSE_ERR_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &center))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_float(tokens[2], &diameter))
		return (PARSE_ERR_INVALID_NUM);
	if (diameter <= 0.0f)
		return (PARSE_ERR_RANGE);
	if (!parse_color(tokens[3], &color))
		return (PARSE_ERR_INVALID_NUM);
	return (init_sphere(ctx, center, diameter)); // TODO: material parsing
}
