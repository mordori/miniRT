/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:47:36 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:47:38 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

static inline t_vec3	get_key_input(mlx_t *mlx);
static inline t_vec3	compute_move_vec(t_camera *cam, t_vec3 input, float dt);
static inline void	clamp_camera(t_camera *cam);

bool	camera_movement(t_context *ctx)
{
	t_camera	*cam;
	t_vec3		input;
	t_vec3		move;
	float		dt;

	if (ctx->renderer.mode == SOLID)
		return (false);
	cam = &ctx->scene.cam;
	input = get_key_input(ctx->mlx);
	if (input.x == 0.0f && input.y == 0.0f && input.z == 0.0f)
		return (false);
	dt = fminf(ctx->mlx->delta_time, 0.1f);
	move = compute_move_vec(cam, input, dt);
	cam->transform.pos = vec3_add(cam->transform.pos, move);
	clamp_camera(cam);
	update_camera(ctx, cam);
	return (true);
}

static inline void	clamp_camera(t_camera *cam)
{
	cam->transform.pos.v = _mm_max_ps(cam->transform.pos.v, g_world_limit_neg.v);
	cam->transform.pos.v = _mm_min_ps(cam->transform.pos.v, g_world_limit.v);
}

static inline t_vec3	get_key_input(mlx_t *mlx)
{
	t_vec3		input;

	input.x = (float)mlx_is_key_down(mlx, KEY_RIGHT)
		- (float)mlx_is_key_down(mlx, KEY_LEFT);
	input.y = (float)mlx_is_key_down(mlx, KEY_UP)
		- (float)mlx_is_key_down(mlx, KEY_DOWN);
	input.z = (float)mlx_is_key_down(mlx, KEY_FORWARD)
		- (float)mlx_is_key_down(mlx, KEY_BACK);
	return (input);
}

static inline t_vec3	compute_move_vec(t_camera *cam, t_vec3 input, float dt)
{
	t_vec3		move_x;
	t_vec3		move_z;
	t_vec3		vec;

	move_x = cam->control_right;
	move_z = cam->control_forward;
	vec = (t_vec3){{0.0f, input.y, 0.0f}};
	vec = vec3_add(vec, vec3_scale(move_x, input.x));
	vec = vec3_add(vec, vec3_scale(move_z, input.z));
	return (vec3_scale(vec, SENS_MOVE * dt));
}
