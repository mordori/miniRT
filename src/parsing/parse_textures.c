#include "libft_io.h"
#include "libft_str.h"
#include "lights.h"
#include "parsing.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static inline bool	is_png_filename(const char *str)
{
	size_t	len;

	if (!str)
		return (false);
	len = ft_strlen(str);
	return (len >= 5 && ft_strcmp(str + len - 4, ".png") == 0);
}

/**
 * Load a PNG texture from assets/textures/ directory.
 */
t_error	load_texture_file(const char *filename, t_texture *out)
{
	char		*path;
	t_texture	tex;

	if (is_placeholder(filename))
	{
		*out = (t_texture){0};
		return (E_OK);
	}
	if (!is_png_filename(filename))
		return (E_TEXTURE);
	path = ft_strjoin(TEXTURE_PATH, filename);
	if (!path)
		return (E_MALLOC);
	tex = load_texture(path, true);
	free(path);
	if (!tex.pixels)
		return (E_TEXTURE);
	*out = tex;
	return (E_OK);
}

/**
 * Find texture by name in parser's texture registry.
 */
t_texture	*find_texture_by_name(t_scene *scene, const char *name)
{
	int	i;

	i = 0;
	while (i < scene->assets.tex_count)
	{
		if (scene->assets.textures[i].loaded
			&& ft_strcmp(scene->assets.textures[i].name, name) == 0)
			return (&scene->assets.textures[i].texture);
		i++;
	}
	return (NULL);
}

/**
 * Parse texture definition: tex <name> <filename.png>
 */
t_error	parse_texture_def(t_context *ctx, char **tokens)
{
	t_tex_entry	*entry;
	t_error		err;

	if (count_tokens(tokens) != 3)
		return (E_MISSING_ARGS);
	if (ctx->scene.assets.tex_count >= MAX_TEXTURES)
		return (E_TOO_MANY);
	if (find_texture_by_name(&ctx->scene, tokens[1]))
		return (E_DUPLICATE);
	entry = &ctx->scene.assets.textures[ctx->scene.assets.tex_count];
	ft_strlcpy(entry->name, tokens[1], MAX_NAME_LEN);
	err = load_texture_file(tokens[2], &entry->texture);
	if (err != E_OK)
		return (err);
	entry->loaded = true;
	ctx->scene.assets.tex_count++;
	return (E_OK);
}
