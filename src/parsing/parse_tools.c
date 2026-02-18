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
	if (*str == '\0' || *str == '.')
		return (false);
	value = ft_atof(str, &end);
	if (end == str || !end)
		return (false);
	while (ft_isspace(*end))
		end++;
	if (isnan(value) || isinf(value) || *end != '\0')
		return (false);
	*out = value;
	return (true);
}

bool	parse_uint(char *str, uint32_t *out)
{
	long	value;
	int		i;

	if (!str || !out)
		return (false);
	while (ft_isspace(*str))
		str++;
	if (*str == '\0')
		return (false);
	i = 0;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] != '\0' || i == 0)
		return (false);
	value = ft_atoi(str);
	if (value < 0)
		return (false);
	*out = (uint32_t)value;
	return (true);
}

bool	parse_int(char *str, int *out)
{
	long	value;
	int		i;

	if (!str || !out)
		return (false);
	while (ft_isspace(*str))
		str++;
	if (*str == '\0')
		return (false);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] != '\0')
		return (false);
	value = ft_atoi(str);
	*out = (int)value;
	return (true);
}

bool	parse_color(char *str, t_vec3 *color)
{
	char	**tokens;
	bool	ret;

	ret = false;
	tokens = ft_split(str, ',');
	if (!tokens)
		return (false);
	if (count_tokens(tokens) == 3 && count_delimiter(str, ',') == 2)
	{
		if (parse_float(tokens[0], &(color->r))
			&& parse_float(tokens[1], &(color->g))
			&& parse_float(tokens[2], &(color->b)))
		{
			if (color->r >= 0 && color->r <= 255 && color->g >= 0
				&& color->g <= 255 && color->b >= 0 && color->b <= 255)
			{
				color->r = color->r / 255.0f;
				color->g = color->g / 255.0f;
				color->b = color->b / 255.0f;
				ret = true;
			}
		}
	}
	free_tokens(tokens);
	return (ret);
}

// Parse a vec3 from a string in the format "x,y,z"
bool	parse_vec3(char *str, t_vec3 *vec)
{
	char	**tokens;
	bool	ret;

	ret = false;
	if (!str || !vec)
		return (ret);
	*vec = (t_vec3){0};
	tokens = ft_split(str, ',');
	if (!tokens)
		return (ret);
	if (count_tokens(tokens) == 3 && count_delimiter(str, ',') == 2)
	{
		ret = parse_float(tokens[0], &vec->x) && parse_float(tokens[1], &vec->y)
			&& parse_float(tokens[2], &vec->z);
		vec->data[3] = 0.0f;
	}
	free_tokens(tokens);
	return (ret);
}
