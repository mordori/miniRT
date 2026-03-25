/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:53:13 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:53:19 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "ui.h"

void	set_text_color(mlx_image_t *img, uint32_t color)
{
	uint32_t	i;
	uint32_t	pixels;

	pixels = img->width * img->height * 4u;
	i = 0;
	while (i < pixels)
	{
		if (img->pixels[i + 3] > 0)
		{
			img->pixels[i + 0] = (color >> 24) & 0xFF;
			img->pixels[i + 1] = (color >> 16) & 0xFF;
			img->pixels[i + 2] = (color >> 8) & 0xFF;
		}
		i += 4;
	}
}

t_ui	ui_val(int32_t *last, int32_t current)
{
	t_ui	values;

	values.last = last;
	values.current = current;
	return (values);
}

t_ui	ui_valf(float *lastf, float currentf)
{
	t_ui	values;

	values.lastf = lastf;
	values.currentf = currentf;
	return (values);
}
