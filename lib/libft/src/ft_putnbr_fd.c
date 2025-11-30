/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:13:06 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 18:40:40 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

#include "libft_io.h"

/**
 * Outputs integer `n` to the specified file descriptor.
 *
 * @param n Integer to output.
 * @param fd File descriptor.
 * @return True if successful, else ERROR.
 */
int	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n == INT_MIN)
	{
		ft_putstr_fd("-2147483648", fd);
		return (true);
	}
	if (n < 0)
	{
		if (write (fd, "-", 1) == ERROR)
			return (ERROR);
		n = -n;
	}
	if (n >= 10)
		if (ft_putnbr_fd(n / 10, fd) == ERROR)
			return (ERROR);
	c = n % 10 + '0';
	if (write (fd, &c, 1) == ERROR)
		return (ERROR);
	return (true);
}
