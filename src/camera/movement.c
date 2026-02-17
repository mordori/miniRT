#include "camera.h"

static inline t_vec3	get_key_input(mlx_t *mlx);
static inline t_vec3	compute_move_vec(t_camera *cam, t_vec3 input, float dt);

bool	camera_movement(t_context *ctx)
{
	t_camera	*cam;
	t_vec3		input;
	t_vec3		move;

	cam = &ctx->scene.cam;
	if (cam->state != CAM_DEFAULT)
		return (false);
	input = get_key_input(ctx->mlx);
	if (input.x == 0.0f && input.y == 0.0f && input.z == 0.0f)
		return (false);
	move = compute_move_vec(cam, input, ctx->mlx->delta_time);
	cam->transform.pos = vec3_add(cam->transform.pos, move);
	update_camera(ctx, cam);
	return (true);
}

static inline t_vec3	get_key_input(mlx_t *mlx)
{
	t_vec3	input;

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
	t_vec3	move_x;
	t_vec3	move_z;
	t_vec3	vec;

	move_x = vec3_normalize((t_vec3){{cam->right.x, 0.0f, cam->right.z}});
	move_z = vec3_normalize((t_vec3){{cam->forward.x, 0.0f, cam->forward.z}});
	vec = (t_vec3){{0.0f, input.y, 0.0f}};
	vec = vec3_add(vec, vec3_scale(move_x, input.x));
	vec = vec3_add(vec, vec3_scale(move_z, input.z));
	return (vec3_scale(vec, SENS_MOVE * dt));
}
