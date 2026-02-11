#include "parsing.h"
#include "libft_io.h"
#include "libft_str.h"
#include "libft_utils.h"
#include "materials.h"
#include "objects.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

t_error	parse_skydome(t_context *ctx, char **tokens)
{
	if (count_tokens(tokens) != 2)
		return (E_MISSING_ARGS);
	return (load_texture_file(tokens[1], &ctx->scene.skydome));
}

inline bool	is_placeholder(const char *str)
{
	return (str && ft_strcmp(str, "_") == 0);
}

void	free_tokens(char **tokens)
{
	ft_free_split(tokens);
}

/**
 * Create area light from emissive material attached to object.
 */
// t_error	create_area_light(t_context *ctx, t_object *obj, t_material *mat)
// {
// 	t_light	*light;

// 	if (!mat->is_emissive)
// 		return (E_OK);
// 	light = malloc(sizeof(*light));
// 	if (!light)
// 		return (E_MALLOC);
// 	*light = (t_light){0};
// 	light->type = LIGHT_AREA;
// 	light->pos = obj->transform.pos;
// 	light->emission = mat->emission;
// 	light->color = mat->albedo;
// 	light->intensity = vec3_length(mat->emission);
// 	light->obj = obj;
// 	vector_try_add(ctx, &ctx->scene.lights, light);
// 	return (E_OK);
// }
