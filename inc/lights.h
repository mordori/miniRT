/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:45:42 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:45:44 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHTS_H
# define LIGHTS_H

# include "defines.h"
# include "parsing.h"

t_error	init_point_light(t_context *ctx, t_light *light, uint32_t mat_id);
void	update_light_radius(t_context *ctx);

#endif
