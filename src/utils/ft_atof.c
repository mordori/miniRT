#include "libft_utils.h"
#include "libft_string.h"

static void after_dot(const char **str, float *fraction, int *divisor)
{
    (*str)++;
    while (ft_isdigit(**str))
    {
        *fraction = *fraction * 10.0f + (**str - 48);
        *divisor *= 10;
        (*str)++;
    }
}

float ft_atof(const char *str)
{
    float result;
    float sign;
    float fraction;
    int divisor;

    result = 0.0f;
    sign = 1.0f;
    fraction = 0.0f;
    divisor = 1;
    while (ft_isspace(*str))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1.0f;
        str++;
    }
    while (ft_isdigit(*str))
        result = result * 10.0f + (*str++ - 48);
    if (*str == '.')
        after_dot(&str, &fraction, &divisor);
    result += fraction / divisor;
    return (result * sign);
}
