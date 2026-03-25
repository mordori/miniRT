/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:19:45 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 19:19:47 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

bool	get_thread_status(t_renderer *r, uint32_t *tile_id)
{
	if (!r->active)
	{
		pthread_mutex_unlock(&r->mutex);
		return (false);
	}
	*tile_id = r->tile_index++;
	++r->threads_running;
	return (true);
}

bool	russian_roulette(t_path *path, t_pixel *pixel)
{
	float		p;

	if (path->bounce >= DEPTH_ENABLE_RR)
	{
		p = \
fmaxf(path->throughput.r, fmaxf(path->throughput.g, path->throughput.b));
		p = clampf(p, 0.05f, 0.95f);
		if (randomf01(pixel->seed) > p)
			return (false);
		path->throughput = vec3_scale(path->throughput, 1.0f / p);
	}
	++path->bounce;
	return (true);
}
