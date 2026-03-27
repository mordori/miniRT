/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:20:43 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/27 20:43:18 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "rendering.h"

void	copy_frame_buffer(\
const t_context *ctx, t_vec3 *buf, uint32_t *pixels, t_pixel *pixel)
{
	const uint32_t		limit = ctx->renderer.pixels;
	const uint32_t		width = ctx->renderer.width;
	t_vec3				color;
	uint32_t			i;

	i = 0;
	while (i < limit)
	{
		color = vec3_scale(buf[i], pixel->scale);
		color = post_process(ctx, pixel, color);
		pixels[i++] = vec3_to_uint32(color);
		if (pixel->x++ == width)
		{
			pixel->x = 0;
			pixel->y++;
		}
	}
}

void	copy_frame_buffer_preview(\
const t_context *ctx, const uint32_t width, t_vec3 *buf, uint32_t *pixels)
{
	const float			*m = ctx->editor.selection_mask;
	const uint32_t		limit = ctx->renderer.pixels;
	uint32_t			i;
	uint32_t			mask;
	uint32_t			edge_color;

	edge_color = color_wave(0xFF007BFF, 0xFFFFFFFF, 10.0f);
	i = 0;
	while (i < limit)
	{
		if (m[i] > 0.0f)
		{
			mask = 0u - ((\
(i % width && m[i - 1] < 0.0f && fabsf(m[i - 1]) > m[i] - 0.05f) || \
((i + 1) % width && m[i + 1] < 0.0f && fabsf(m[i + 1]) > m[i] - 0.05f) || \
(i >= width && m[i - width] < 0.0f && fabsf(m[i - width]) > m[i] - 0.05f) || \
(i + width < limit && m[i + width] < 0.0f && \
fabsf(m[i + width]) > m[i] - 0.05f)));
		}
		else
			mask = 0u;
		pixels[i] = \
(edge_color & mask) | \
(vec3_to_uint32(post_process_preview(ctx, buf[i])) & ~mask);
		++i;
	}
}

void	print_render_status(t_context *ctx, t_renderer *r)
{
	const int		bar_width = 20;
	static char		buf[128];
	static char		bar[32];
	int				hashes;
	int				i;

	hashes = (r->frame * bar_width) / r->render_samples;
	if (hashes > bar_width)
		hashes = bar_width;
	i = 0;
	while (i < bar_width)
	{
		if (i < hashes)
			bar[i] = '#';
		else
			bar[i] = ' ';
		++i;
	}
	bar[bar_width] = '\0';
	snprintf(buf, sizeof(buf), \
"\r\033[K\033[1;33mRendering...   [%s] [%u/%u]\033[0m", bar, r->frame, \
r->render_samples);
	try_write(ctx, STDOUT_FILENO, buf);
}

void	limit_polling_rate(t_renderer *r)
{
	static uint32_t		last_frame_time;

	if (r->frame < r->render_samples || r->mode != RENDERED)
		return ;
	wait_until(last_frame_time + 17);
	last_frame_time = time_now();
}

uint32_t	color_wave(uint32_t c1, uint32_t c2, float speed)
{
	float		time;
	float		wave;
	float		t;

	time = engine_time() / 1000.0f;
	wave = sinf(time * speed);
	t = (wave + 1.0f) * 0.5f;
	return (vec3_to_uint32(lerp_color(c1, c2, t)));
}
