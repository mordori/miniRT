#include "libft_str.h"
#include "libft_utils.h"

static float	skip_whitespace_and_sign(const char **str)
{
	while (ft_isspace(**str))
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
		{
			(*str)++;
			return (-1.0f);
		}
		(*str)++;
	}
	return (1.0f);
}

static float	parse_integer_part(const char **str)
{
	float	result;

	result = 0.0f;
	while (ft_isdigit(**str))
		result = result * 10.0f + (*(*str)++ - 48);
	return (result);
}

static float	parse_fraction(const char **str)
{
	float	fraction;
	int		divisor;

	fraction = 0.0f;
	divisor = 1;
	(*str)++;
	while (ft_isdigit(**str))
	{
		fraction = fraction * 10.0f + (**str - 48);
		divisor *= 10;
		(*str)++;
	}
	return (fraction / divisor);
}

static void	set_end_pointer(const char *start, const char *current,
		char **endptr)
{
	if (!endptr || !start || !current)
		return ;
	if (current == start || (current == start + 1 && (*start == '-'
				|| *start == '+')))
		*endptr = (char *)start;
	else
		*endptr = (char *)current;
}

float	ft_atof(const char *str, char **endptr)
{
	float		result;
	float		sign;
	const char	*start;

	if (!str || !endptr)
		return (0.0f);
	start = str;
	sign = skip_whitespace_and_sign(&str);
	result = parse_integer_part(&str);
	if (*str == '.')
		result += parse_fraction(&str);
	set_end_pointer(start, str, endptr);
	return (result * sign);
}
