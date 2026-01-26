#include "libft_io.h"
#include "libft_str.h"
#include "lights.h"
#include "parsing.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

// Check if filename ends with .png
bool	is_png_filename(const char *str)
{
	size_t	len;

	if (!str)
		return (false);
	len = ft_strlen(str);
	if (len < 5)
		return (false);
	return (ft_strcmp(str + len - 4, ".png") == 0);
}

// Load a texture from file into out parameter
t_error load_texture_file(t_context *ctx, const char *filename,
				t_texture *out)
{
	char	*path;
	t_texture	tex;

	if (!filename || !out)
		return(PARSE_ERR_MALLOC);
	if (!is_png_filename(filename))
		return (PARSE_ERR_TEXTURE);
	path = ft_strjoin(TEXTURE_PATH, filename);
	if (!path)
		return (PARSE_ERR_MALLOC);
	tex = load_texture(ctx, path, true); // gamma-corrected
	free(path);
	if (!tex.pixels)
		return (PARSE_ERR_TEXTURE);
	*out = tex;
	return (PARSE_OK);
}

t_texture	*find_texture_name(t_parser *p, const char *name)
{
	int	i;

	if (!p || !name)
		return (NULL);
	i = 0;
	while(i < p->texture_count)
	{
		if (p->textures[i].loaded && ft_strcmp(p->textures[i].name, name) == 0)
			return(&p->textures[i].texture);
		i++;
	}
	return(NULL);
}

// tex <name> <filename.png>
t_error	parse_texture(t_context *ctx, t_parser *p, char **tkns)
{
	t_tex	*entry;
	t_error	err;

	if (count_tokens(tkns) != 3)
		return(PARSE_ERR_MISSING_ARGS);
	if (p->texture_count >= MAX_TEXTURES)
		return (PARSE_ERR_TOO_MANY);
	if (find_texture_name(p, tkns[1])) // check duplicate
		return(PARSE_ERR_DUPLICATE);
	entry = &p->textures[p->texture_count];
	ft_strlcpy(entry->name, tkns[1], MAX_NAME_LEN);
	err = load_texture_file(ctx, tkns[2], &entry->texture);
	if (err != PARSE_OK)
		return (err);
	entry->loaded = true;
	p->texture_count++;
	return (PARSE_OK);
}

// sky <filename.png>
// Loads skydome texture directly into scene
t_error	parse_skydome(t_context *ctx, char **tkns)
{
	t_error	err;

	if (count_tokens(tkns) != 2)
		return (PARSE_ERR_MISSING_ARGS);
	if (ctx->scene.skydome.pixels)
		return (PARSE_ERR_DUPLICATE);
	err = load_texture_file(ctx, tkns[1], &ctx->scene.skydome);
	return (err);
}
