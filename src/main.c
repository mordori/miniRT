/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 19:44:39 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 00:42:34 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "utils.h"

static inline void	initialize(t_context *ctx);
static inline void	update(void *param);
static inline void	clean(t_context *ctx);

int	main(int argc, char *argv[])
{
	t_context	ctx;
	(void)argc;
	(void)argv;

	// validate file name (.rt)
	initialize(&ctx);
	clean(&ctx);
	return (0);
}

static inline void	initialize(t_context *ctx)
{
	*ctx = (t_context){0};
	ctx->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!ctx->mlx)

}

static inline void	update(void *param)
{
	t_context	*ctx;

	ctx = (t_context *)param;
	// input();
	// cam_update();
}

static inline void	clean(t_context *ctx)
{

}
