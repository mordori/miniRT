#include "camera.h"
#include "rendering.h"
#include "utils.h"

static inline void	update_viewport(t_camera *cam, float img_width, float img_height);

void	init_camera(t_context *ctx, t_vec3 position, t_vec3 orientation,
		float fov)
{
	t_camera	*cam;

	if (!ctx)
		return ;
	cam = &ctx->scene.cam;
	cam->state = CAM_DEFAULT;
	cam->transform.pos = position;
	cam->target.pos = vec3_add(position, orientation);
	cam->pivot = (t_vec3){0};
	cam->yaw = atan2f(orientation.x, orientation.z);
	cam->pitch = asinf(orientation.y);
	cam->distance = 1.0f;
	fov = degrees_to_rad(fov);
	fov = clampf(fov, degrees_to_rad(0.1f), degrees_to_rad(179.9f));
	cam->focal_len_mm = SENSOR_HEIGHT_MM * 0.5f / tanf(fov * 0.5f);
	cam->f_stop = 0.3f;
	cam->focus_dist = 1.3f;
	cam->init_pos = position;
}

t_vec3	sample_defocus_disk(const t_context *ctx, t_pixel *pixel)
{
	t_vec2		uv;
	t_vec3		disk;
	t_vec3		res;
	t_vec3		u;
	t_vec3		v;

	uv = vec2(blue_noise(&ctx->tex_bn, pixel, BN_DD_U), blue_noise(&ctx->tex_bn, pixel, BN_DD_V));
	disk = sample_disk(uv);
	u = vec3_scale(ctx->renderer.cam.defocus_disk_u, disk.x);
	v = vec3_scale(ctx->renderer.cam.defocus_disk_v, disk.y);
	res = vec3_add(u, v);
	res = vec3_add(ctx->renderer.cam.transform.pos, res);
	return (res);
}

void	update_camera(t_context *ctx, t_camera *cam)
{
	static const t_vec3	world_up = {{0.0f, 1.0f, 0.0f}};
	const float			limit = M_PI_2 - 0.001f;
	t_vec3				dir;
	t_vec2				pitch;
	t_vec2				yaw;

	cam->pitch = clampf(cam->pitch, -limit, limit);
	cam->aspect = (float)ctx->img->width / ctx->img->height;
	if (ctx->selected_obj)
		cam->pivot = ctx->selected_obj->transform.pos;
	else
		cam->pivot = (t_vec3){0};
	sincosf(cam->pitch, &pitch.sin, &pitch.cos);
	sincosf(cam->yaw, &yaw.sin, &yaw.cos);
	dir.x = pitch.cos * yaw.sin;
	dir.y = pitch.sin;
	dir.z = pitch.cos * yaw.cos;
	cam->forward = vec3_normalize(dir);
	cam->right = vec3_normalize(vec3_cross(world_up, cam->forward));
	cam->up = vec3_normalize(vec3_cross(cam->forward, cam->right));
	update_viewport(cam, (float)ctx->img->width, (float)ctx->img->height);
}

static inline void	update_viewport(t_camera *cam, float img_width, float img_height)
{
	t_viewport			*vp;
	t_vec3				u;
	t_vec3				v;
	t_vec3				vp_center;
	t_vec3				vp_up_left;
	float				defocus_r;
	float				fov_vertical;

	vp = (t_viewport *)&cam->viewport;

	fov_vertical = 2.0f * atanf(SENSOR_HEIGHT_MM / (2.0f * cam->focal_len_mm));
	vp->height = 2.0f * tanf(fov_vertical * 0.5f) * cam->focus_dist;


	// vp->height = 2.0f * tanf(cam->focal_len * 0.5f) * cam->focus_dist;
	vp->width = vp->height * cam->aspect;
	u = vec3_scale(cam->right, vp->width);
	v = vec3_scale(cam->up, -vp->height);
	vp->d_u = vec3_div(u, img_width);
	vp->d_v = vec3_div(v, img_height);
	vp_center = vec3_add(cam->transform.pos, vec3_scale(cam->forward,
				cam->focus_dist));
	vp_up_left = vec3_sub(vp_center, vec3_scale(u, 0.5f));
	vp_up_left = vec3_sub(vp_up_left, vec3_scale(v, 0.5f));
	vp->pixel_00_loc = vp_up_left;


	if (cam->f_stop > 0.0f)
		defocus_r = cam->focal_len_mm * 0.001f / (2.0f * cam->f_stop);
	else
		defocus_r = 0.0f;


	// defocus_r = cam->focus_dist * tanf(degrees_to_rad(cam->aperture * 0.5f));



	cam->defocus_disk_u = vec3_scale(cam->right, defocus_r);
	cam->defocus_disk_v = vec3_scale(cam->up, defocus_r);
}
