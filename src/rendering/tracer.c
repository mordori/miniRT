#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "utils.h"

static inline t_vec4	background_color(const t_texture *tex, const t_ray *ray);

t_vec4	trace_ray(const t_scene *scene, uint32_t x, uint32_t y)
{
	const t_viewport	*vp;
	t_ray				ray;
	t_hit				hit;
	t_vec3				pixel_loc;
	t_vec3				offset;

	vp = &scene->cam.viewport;
	offset = vec3_add(vec3_scale(vp->d_u, x), vec3_scale(vp->d_v, y));
	pixel_loc = vec3_add(vp->pixel_00_loc, offset);
	ray = new_ray(scene->cam.transform.pos, pixel_loc);
	hit = (t_hit){0};
	hit.t = M_INF;
	if (hit_bvh(scene->bvh_root, &ray, &hit))
		return (calculate_lighting(scene, &hit, (size_t)x, (float)y));
	return (background_color(&scene->skydome, &ray));
}

t_ray	new_ray(t_vec3 origin, t_vec3 dest)
{
	t_ray		ray;

	ray.origin = origin;
	ray.dir = vec3_normalize(vec3_sub(dest, ray.origin));
	if (fabsf(ray.dir.x) < M_EPSILON)
		ray.inv_dir.x = copysignf(M_INF, ray.dir.x);
	else
		ray.inv_dir.x = 1.0f / ray.dir.x;
	if (fabsf(ray.dir.y) < M_EPSILON)
		ray.inv_dir.y = copysignf(M_INF, ray.dir.y);
	else
		ray.inv_dir.y = 1.0f / ray.dir.y;
	if (fabsf(ray.dir.z) < M_EPSILON)
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

	if (!tex || !tex->pixels)
		return ((t_vec4){{0.0f, 0.0f, 0.0f, 1.0f}});
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
