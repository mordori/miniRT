/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:47:33 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:47:35 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "input.h"

void	cam_look(t_context *ctx, t_vec2i delta, float speed)
{
	t_camera	*cam;

	cam = &ctx->scene.cam;
	speed *= SENS_LOOK * (14.0f / cam->focal_len_mm);
	cam->yaw += (float)delta.x * speed;
	cam->pitch -= (float)delta.y * speed;
	cam->pitch = clampf(cam->pitch, -M_PI_2 + 0.001f, M_PI_2 - 0.001f);
	cam->transform.rot = quat_from_euler(vec3(-cam->pitch, cam->yaw, 0.0f));
	update_camera(ctx, cam);
}

void	cam_turn(t_context *ctx, t_vec2i delta, float speed)
{
	cam_look(ctx, delta, speed);
	ctx->scene.cam.control_forward = ctx->scene.cam.forward;
	ctx->scene.cam.control_right = ctx->scene.cam.right;
}

void	cam_orbit(t_context *ctx, t_vec2i delta, float speed)
{
	t_camera	*cam;
	t_quat		pitch;
	t_quat		yaw;
	t_vec3		orbit_center;
	t_vec3		diff;
	t_vec3		local;

	cam = &ctx->scene.cam;
	speed *= SENS_ORBIT;
	if (ctx->editor.selected_obj)
		orbit_center = ctx->editor.selected_obj->transform.pos;
	else
		orbit_center = vec3_add(cam->transform.pos, vec3_scale(cam->forward, cam->distance));
	diff = vec3_sub(orbit_center, cam->transform.pos);
	cam->distance = fmaxf(vec3_length(diff), 0.01f);
	local = vec3(vec3_dot(diff, cam->right), vec3_dot(diff, cam->up), vec3_dot(diff, cam->forward));
	yaw = quat_from_euler_angle(g_up, (float)delta.x * speed);
	pitch = quat_from_euler_angle(cam->right, (float)delta.y * speed);
	cam->transform.rot = quat_mul(yaw, cam->transform.rot);
	cam->transform.rot = quat_mul(pitch, cam->transform.rot);
	update_camera(ctx, cam);
	diff = vec3_add(vec3_scale(cam->right, local.x), vec3_scale(cam->up, local.y));
	diff = vec3_add(diff, vec3_scale(cam->forward, local.z));
	cam->transform.pos = vec3_sub(orbit_center, diff);
	ctx->scene.cam.control_forward = ctx->scene.cam.forward;
	ctx->scene.cam.control_right = ctx->scene.cam.right;
}


void	cam_zoom(t_context *ctx, t_vec2i delta, float speed)
{
	t_camera	*cam;
	float		move;

	cam = &ctx->scene.cam;
	speed *= SENS_ZOOM * cam->distance;
	move = (float)delta.y * speed;
	cam->transform.pos = vec3_add(cam->transform.pos, vec3_scale(cam->forward, move));
	cam->distance = fmaxf(cam->distance - move, 0.01f);
}

void	cam_pan(t_context *ctx, t_vec2i delta, float speed)
{
	t_camera	*cam;
	t_vec3		move;

	cam = &ctx->scene.cam;
	speed *= SENS_PAN * cam->distance;
	move = vec3_add(
		vec3_scale(cam->right, -(float)delta.x * speed),
		vec3_scale(cam->up, (float)delta.y * speed)
	);
	cam->transform.pos = vec3_add(cam->transform.pos, move);
}
