/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:19:38 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 19:19:40 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	update_transform(t_transform *t)
{
	t_mat4		translation;
	t_mat4		rotation;
	t_mat4		scale;

	translation = mat4_translate(t->pos);
	rotation = quat_to_mat4(t->rot);
	scale = mat4_scale(t->scale);
	t->object_to_world = mat4_mul(&translation, &rotation);
	t->object_to_world = mat4_mul(&t->object_to_world, &scale);
	mat4_inverse(&t->object_to_world, &t->world_to_object);
}
