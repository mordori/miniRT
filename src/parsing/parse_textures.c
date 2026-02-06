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
t_error	load_texture_file(t_context *ctx, const char *filename, t_texture *out)
{
	char		*path;
	t_texture	tex;

	if (is_placeholder(filename))
	{
		*out = (t_texture){0};
		return (PARSE_OK);
	}
	if (!is_png_filename(filename))
		return (PARSE_ERR_TEXTURE);
	path = ft_strjoin(TEXTURE_PATH, filename);
	if (!path)
		return (PARSE_ERR_MALLOC);
	tex = load_texture(ctx, path, true);
	free(path);
	if (!tex.pixels)
		return (PARSE_ERR_TEXTURE);
	*out = tex;
	return (PARSE_OK);
}

/**
 * Find texture by name in parser's texture registry.
 */
t_texture	*find_texture_by_name(t_parser *p, const char *name)
{
	int	i;

	i = 0;
	while (i < p->tex_count)
	{
		if (p->textures[i].loaded && ft_strcmp(p->textures[i].name, name) == 0)
			return (&p->textures[i].texture);
		i++;
	}
	return (NULL);
}

/**
 * Parse texture definition: tex <name> <filename.png>
 */
t_error	parse_texture_def(t_context *ctx, t_parser *p, char **tokens)
{
	t_tex_entry	*entry;
	t_error		err;

	if (count_tokens(tokens) != 3)
		return (PARSE_ERR_MISSING_ARGS);
	if (p->tex_count >= MAX_TEXTURES)
		return (PARSE_ERR_TOO_MANY);
	if (find_texture_by_name(p, tokens[1]))
		return (PARSE_ERR_DUPLICATE);
	entry = &p->textures[p->tex_count];
	ft_strlcpy(entry->name, tokens[1], MAX_NAME_LEN);
	err = load_texture_file(ctx, tokens[2], &entry->texture);
	if (err != PARSE_OK)
		return (err);
	entry->loaded = true;
	p->tex_count++;
	return (PARSE_OK);
}
