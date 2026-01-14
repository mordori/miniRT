#include "libft_io.h"
#include "libft_str.h"
#include "libft_utils.h"
#include "parsing.h"
#include "utils.h"
#include <math.h>
#include <stdlib.h>

bool	parse_float(char *str, float *out)
{
	char	*end;
	float	value;

	if (!str || !out)
		return (false);
	end = NULL;
	while (ft_isspace(*str))
		str++;
	if (*str == '\0')
		return (false);
	value = ft_atof(str, &end);
	if (end == str)
		return (false);
	while (ft_isspace(*end))
		end++;
	if (*end != '\0')
		return (false);
	// if (isnan(value) || isinf(value)) // check for NaN or Infinity
	// 	return (false);
	*out = value;
	return (true);
}

bool	parse_color(char *str, t_vec3 *color)
{
	char	**tokens;
	float	r;
	float	g;
	float	b;
	bool	ret;

	if (!str || !color)
		return (false);
	tokens = ft_split(str, ',');
	ret = false;
	if (!tokens)
		return (false);
	if (count_tokens(tokens) >= 3 && count_tokens(tokens) <= 4)
	{
		if (parse_float(tokens[0], &r) && parse_float(tokens[1], &g)
			&& parse_float(tokens[2], &b))
		{
			if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)
			{
				color->r = r / 255.0f;
				color->g = g / 255.0f;
				color->b = b / 255.0f;
				// color->a = 1.0f; // fully opaque
				ret = true;
			}
		}
	}
	free_tokens(tokens);
	return (ret);
}

// t_parse_error parse_material(t_parser *p, const char *token, t_material *mat)
// {
// }

// Parse a vec3 from a string in the format "x,y,z"
bool	parse_vec3(char *str, t_vec3 *vec)
{
	char	**tokens;
	bool	ret;

	ret = false;
	if (!str || !vec)
		return (ret);
	tokens = ft_split(str, ',');
	if (!tokens)
		return (ret);
	if (count_tokens(tokens) >= 3 && count_tokens(tokens) <= 4)
	{
		ret = parse_float(tokens[0], &vec->x) && parse_float(tokens[1], &vec->y)
			&& parse_float(tokens[2], &vec->z);
	}
	free_tokens(tokens);
	return (ret);
}

int	count_tokens(char **tokens)
{
	int	count;

	if (!tokens)
		return (0);
	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

// a wrapper to free the tokens array
void	free_tokens(char **tokens)
{
	ft_free_split(tokens);
}
