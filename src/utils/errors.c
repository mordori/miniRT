#include "utils.h"
#include "libft_io.h"
#include "libft_utils.h"

void	fatal_error(t_context *ctx, char *msg, char *file, int line)
{
	char	str[INT32_LENGTH + 1];

	ft_putstr_fd("Error:\n", STDERR_FILENO);
	if (msg)
		ft_putendl_fd(msg, STDERR_FILENO);
	if (file)
	{
		ft_putstr_fd(file, STDERR_FILENO);
	}
	if (ctx && line > 0)
	{
		ft_putstr_fd("Line: ", STDERR_FILENO);
		int_to_str(line, str);
		ft_putendl_fd(str, STDERR_FILENO);
	}
	if (ctx && ctx->mlx)
	{
		ft_putstr_fd("MLX42: ", STDERR_FILENO);
		perror(mlx_strerror(mlx_errno));
	}
	clean(ctx);
	exit(EXIT_FAILURE);
}

char	*errors(t_err_code code)
{
	static char	*e[] =
	{
"invalid number of arguments", \
"mlx init failed", \
"mlx img failed", \
"vector init failed", \
"vector add failed", \
"resize failed", \
"write: src is longer than SSIZE_MAX", \
"write file failed", \
"open file failed", \
"read file failed", \
"GNL failed", \
"texture load failed", \
"invalid entity type", \
"renderer init failed", \
"object add failed", \
"point light add failed", \
"bvh failed", \
"material add failed" \
"NPOT texture"
	};

	return (e[code]);
}
