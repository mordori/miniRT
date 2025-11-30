/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:58:22 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/27 00:02:26 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>

#include "libft_io.h"
#include "libft_mem.h"
#include "libft_str.h"
#include "libft_utils.h"

static inline char	*ft_ptrformat(char *s);
static inline char	*ft_strformat(const char c, va_list *args);
static inline ssize_t	ft_strprint(const char c, va_list *args);

/**
 * Formats and prints string `s` with variable type argument list.
 *
 * Supported data type conversions:
 *
 * c s d i u x X p %
 *
 * @param s Source string.
 * @return Number of bytes written, summed from characters written from `s`
 * and the type conversions. Returns 0 on error.
 */
ssize_t	ft_printf(const char *s, ...)
{
	va_list	args;
	ssize_t	totalbytes;
	ssize_t	bytes;

	if (!s)
		return (ERROR);
	va_start(args, s);
	totalbytes = 0;
	while (*s)
	{
		if (*s++ == '%')
			bytes = ft_strprint(*s++, &args);
		else
			bytes = ft_putchar_fd(*(s - 1), STDOUT_FILENO);
		if (bytes == ERROR)
		{
			totalbytes = 0;
			break ;
		}
		totalbytes += bytes;
	}
	va_end(args);
	return ((int)totalbytes);
}

/**
 * Formats pointer address. Expects string 's' as hexadecimal number.
 *
 * @param s Source string.
 * @return String of formatted pointer address in hex, beginning with `0x` or
 * `(nil)` if NULL pointer.
 */
static inline char	*ft_ptrformat(char *s)
{
	char	*str;

	if (!s)
		return (NULL);
	if (*s == '0')
		str = ft_strdup("(nil)");
	else
		str = ft_strjoin("0x", s);
	free (s);
	return (str);
}

/**
 * Formats given argument in `args` with data type specified with `c`.
 * Allocates memory for a new string.
 *
 * @param c Data type specified to be coverted.
 * @param args Variable type argument list.
 * @return String with formatted type conversion.
 */
static inline char	*ft_strformat(const char c, va_list *args)
{
	char	*str;

	if (c == '%')
		str = ft_memset(ft_calloc(2, 1), '%', 1);
	else if (c == 'c')
		str = ft_memset(ft_calloc(2, 1), va_arg(*args, unsigned int), 1);
	else if (c == 's')
		str = ft_strdup(va_arg(*args, char *));
	else if (c == 'd' || c == 'i')
		str = ft_itoa(va_arg(*args, int), BASE_10);
	else if (c == 'u')
		str = ft_uitoa(va_arg(*args, unsigned int), BASE_10);
	else if (c == 'x' || c == 'X')
		str = ft_uitoa(va_arg(*args, unsigned int), BASE_16);
	else if (c == 'p')
		str = ft_ptrformat(ft_uitoa(va_arg(*args, uintptr_t), BASE_16));
	else
		return (NULL);
	if (c == 'p' || c == 'x')
		ft_striteri(str, ft_tolower);
	if (c == 's' && !str)
		str = ft_strdup("(null)");
	return (str);
}

/**
 * Prints and then free()s the formatted string `str`.
 *
 * @param c Data type specified to be coverted.
 * @param args Variable type argument list.
 * @return Number of bytes written or -1 on error.
 */
static inline ssize_t	ft_strprint(const char c, va_list *args)
{
	char	*str;
	ssize_t	bytes;

	if (!args)
		return (ERROR);
	str = ft_strformat(c, args);
	if (!str)
		return (ERROR);
	if (c == 'c')
		bytes = ft_putchar_fd(*str, STDOUT_FILENO);
	else
		bytes = ft_putstr_fd(str, STDOUT_FILENO);
	free(str);
	return (bytes);
}
