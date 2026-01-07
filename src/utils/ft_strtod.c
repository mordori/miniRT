#include "libft_str.h"
#include "libft_utils.h"

static double	skip_whitespace_and_sign(const char **str)
{
	while (ft_isspace(**str))
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
		{
			(*str)++;
			return (-1.0);
		}
		(*str)++;
	}
	return (1.0);
}

static double	parse_integer_part(const char **str)
{
	double	result;

	result = 0.0;
	while (ft_isdigit(**str))
		result = result * 10.0 + (*(*str)++ - 48);
	return (result);
}

static double	parse_fraction(const char **str)
{
	double	fraction;
	int		divisor;

	fraction = 0.0;
	divisor = 1;
	(*str)++;
	while (ft_isdigit(**str))
	{
		fraction = fraction * 10.0 + (**str - 48);
		divisor *= 10;
		(*str)++;
	}
	return (fraction / divisor);
}

static void	set_end_pointer(const char *start, const char *current,
		char **endptr)
{
	if (!endptr)
		return ;
	if (current == start || (current == start + 1 && (*start == '-'
				|| *start == '+')))
		*endptr = (char *)start;
	else
		*endptr = (char *)current;
}

double	ft_strtod(const char *str, char **endptr)
{
	double		result;
	double		sign;
	const char	*start;

	start = str;
	sign = skip_whitespace_and_sign(&str);
	result = parse_integer_part(&str);
	if (*str == '.')
		result += parse_fraction(&str);
	set_end_pointer(start, str, endptr);
	return (result * sign);
}
