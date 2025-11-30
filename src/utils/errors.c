/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 20:28:46 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 01:00:55 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "libft_io.h"

void	fatal_error(t_context *ctx, char *msg)
{
	ft_putstr_fd("miniRT:\tError: ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	if (ctx->mlx)
	{
		ft_putstr_fd("MLX42:\t", STDERR_FILENO);
		perror(mlx_strerror(mlx_errno));
		mlx_terminate(ctx->mlx);
	}
	clean(ctx);
	exit(EXIT_FAILURE);
}

void	warning(t_minishell *ms, char *src)
{
	char	*msg;

	msg = "undefined source";
	if (src)
		msg = src;
	if (ms->mode == INTERACTIVE)
		try_write(ms, STDERR_FILENO, "\001\033[1;33m\002");
	try_write(ms, STDERR_FILENO, "minishell: ");
	if (errno)
	{
		perror(msg);
		errno = 0;
	}
	else
	{
		try_write(ms, STDERR_FILENO, msg);
		try_write(ms, STDERR_FILENO, "\n");
	}
	if (ms->mode == INTERACTIVE)
		try_write(ms, STDERR_FILENO, "\001\033[0m\002");
}
