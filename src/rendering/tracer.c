#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "utils.h"

static inline t_ray		new_ray(const t_viewport *vp, t_vec3 origin, uint32_t x, uint32_t y);
static inline t_vec4	background_color(const t_texture *tex, const t_ray *ray);

t_vec4	trace_ray(const t_scene *scene, uint32_t x, uint32_t y)
{
	t_ray	ray;
	t_hit	hit;

	ray = new_ray(&scene->cam.viewport, scene->cam.transform.pos, x, y);
	hit = (t_hit){0};
	hit.t = M_INF;
	if (hit_bvh(scene->bvh_root, &ray, &hit))
		return (calculate_lighting(scene, &hit));
	return (background_color(&scene->skydome, &ray));
}

// If the direction axis is nearing zero, we explicitly set it to +-FLT_MAX.
// -ffast-math doesn't support inf and float division by 0.
// This also avoids nan in the case of 0/0.
static inline t_ray	new_ray(const t_viewport *vp, t_vec3 origin, uint32_t x, uint32_t y)
{
	t_ray	ray;
	t_vec3	pixel_center;
	t_vec3	offset;

	offset = vec3_add(vec3_scale(vp->d_u, (float)x), vec3_scale(vp->d_v, (float)y));
	pixel_center = vec3_add(vp->pixel_00_loc, offset);
	ray.origin = origin;
	ray.dir = vec3_normalize(vec3_sub(pixel_center, ray.origin));
	if (fabsf(ray.dir.x) < 1e-6f)
		ray.inv_dir.x = copysignf(M_INF, ray.dir.x);
	else
		ray.inv_dir.x = 1.0f / ray.dir.x;
	if (fabsf(ray.dir.y) < 1e-6f)
		ray.inv_dir.y = copysignf(M_INF, ray.dir.y);
	else
		ray.inv_dir.y = 1.0f / ray.dir.y;
	if (fabsf(ray.dir.z) < 1e-6f)
		ray.inv_dir.z = copysignf(M_INF, ray.dir.z);
	else
		ray.inv_dir.z = 1.0f / ray.dir.z;
	ray.sign[0] = (ray.dir.x < 0);
	ray.sign[1] = (ray.dir.y < 0);
	ray.sign[2] = (ray.dir.z < 0);
	return (ray);
}

static inline t_vec4	background_color(const t_texture *tex, const t_ray *ray)
{
	float		phi;
	float		theta;
	t_float2	uv;
	t_int2		xy;
	uint32_t	i;

	if (!tex)
		return ((t_vec4){{1.0f, 0.0f, 0.0f, 1.0f}});
	phi = atan2f(ray->dir.z, ray->dir.x);
	theta = acosf(ray->dir.y);
	uv.u = (phi + M_PI) * M_1_2PI;
	uv.v = theta * M_1_PI;
	xy.x = (uint32_t)(uv.u * (tex->width - 1));
	xy.y = (uint32_t)(uv.v * (tex->height - 1));
	i = (xy.y * tex->width + xy.x) * 4;
	return ((t_vec4){{
		tex->pixels[i],
		tex->pixels[i + 1],
		tex->pixels[i + 2],
		1.0f
	}});
}
