#include "camera.h"
#include "rendering.h"

static inline void	update_viewport(const t_camera *cam, float img_width, float img_height);

void	add_camera(t_context *ctx, char **params)
{
	t_camera	*cam;

	(void)params;
	cam = &ctx->scene.cam;
	cam->state = CAM_DEFAULT;
	cam->transform.pos = (t_vec3){{0.0f, 0.0f, 0.0f}};
	cam->pivot = (t_vec3){0};
	cam->target.pos = (t_vec3){{0.0f, 0.0f, 0.0f}};
	cam->yaw = 0.0f;
	cam->pitch = 0.0f;
	cam->fov = degrees_to_rad(60.0f);
	cam->focal_length = 1.0f;
	cam->distance = vec3_length(vec3_sub(cam->transform.pos, cam->pivot));
}

void	update_camera(t_context *ctx)
{
	static const t_vec3	world_up = {{0.0f, 1.0f, 0.0f}};
	const float			limit = M_PI_2 - 0.001f;
	t_vec3				dir;
	t_camera			*cam;

	cam = &ctx->scene.cam;
	cam->pitch = ft_clamp(cam->pitch, -limit, limit);
	cam->aspect = (float)ctx->img->width / ctx->img->height;
	if (ctx->editor.selected_object)
		cam->pivot = ctx->editor.selected_object->transform.pos;
	else
		cam->pivot = (t_vec3){0};
	dir.x = cosf(cam->pitch) * sinf(cam->yaw);
	dir.y = sinf(cam->pitch);
	dir.z = cosf(cam->pitch) * cosf(cam->yaw);
	cam->forward = vec3_normalize(dir);
	cam->right = vec3_normalize(vec3_cross(world_up, cam->forward));
	cam->up = vec3_normalize(vec3_cross(cam->forward, cam->right));
	update_viewport(cam, (float)ctx->img->width, (float)ctx->img->height);
}

static inline void	update_viewport(const t_camera *cam, float img_width, float img_height)
{
	t_viewport	*vp;
	t_vec3		u;
	t_vec3		v;
	t_vec3		vp_center;
	t_vec3		vp_up_left;

	vp = (t_viewport *)&cam->viewport;
	vp->height = 2.0f * tanf(cam->fov / 2.0f);
	vp->width = vp->height * cam->aspect;
	u = vec3_scale(cam->right, vp->width);
	v = vec3_scale(cam->up, -vp->height);
	vp->d_u = vec3_div(u, img_width);
	vp->d_v = vec3_div(v, img_height);
	vp_center = vec3_add(cam->transform.pos, vec3_scale(cam->forward, cam->focal_length));
	vp_up_left = vec3_sub(vp_center, vec3_div(u, 2.0f));
	vp_up_left = vec3_sub(vp_up_left, vec3_div(v, 2.0f));
	vp->pixel_00_loc = vec3_add(vp_up_left, vec3_scale(vec3_add(vp->d_u, vp->d_v), 0.5f));
}
