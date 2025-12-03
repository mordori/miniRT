#include "utils.h"
#include "libft_io.h"

void	fatal_error(t_context *ctx, char *message)
{
	char	*msg;

	msg = "undefined message";
	if (message)
		msg = message;
	ft_putstr_fd("Error:\n", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	if (ctx && ctx->mlx)
	{
		ft_putstr_fd("MLX42:\t", STDERR_FILENO);
		perror(mlx_strerror(mlx_errno));
	}
	clean(ctx);
	exit(EXIT_FAILURE);
}
