#include "rendering.h"
#include "scene.h"
#include "lights.h"

static inline t_ray	new_ray(const t_viewport *vp, const t_vec3 origin, const uint32_t x, const uint32_t y);
static inline t_vec4	backgroud_color(const t_scene *scene, const t_ray *ray);

t_vec4	trace_ray(const t_scene *scene, const uint32_t x, const uint32_t y)
{
	t_ray	ray;
	t_hit	hit;

	ray = new_ray(&scene->cam.viewport, scene->cam.transform.pos, x, y);
	hit = (t_hit){0};
	hit.t = M_INF;
	if (hit_bvh(scene->bvh_root, &ray, &hit))
		return (calculate_lighting(scene, &hit));
	return (backgroud_color(scene, &ray));
}

// If the direction axis is nearing zero, we explicitly set it to +-FLT_MAX.
// -ffast-math optimization doesn't support infinity and float division by 0.
// This also avoids nan in the case of (0-0) / 0.
static inline t_ray	new_ray(const t_viewport *vp, const t_vec3 origin, const uint32_t x, const uint32_t y)
{
	t_ray	ray;
	t_vec3	pixel_center;
	t_vec3	offset;

	offset = vec3_add(vec3_scale(vp->d_u, (float)x), vec3_scale(vp->d_v, (float)y));
	pixel_center = vec3_add(vp->pixel_00_pos, offset);
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

static inline t_vec4	backgroud_color(const t_scene *scene, const t_ray *ray)
{
	uint32_t	u;
	uint32_t	v;
	uint32_t	width;
	uint32_t	height;
	uint32_t	i;

	if (!scene->skydome)
		return ((t_vec4){{0.2f, 0.5f, 0.0f, 1.0f}});
	width = scene->skydome->width;
	height = scene->skydome->height;
	u = width * atan2f(ray->dir.z, ray->dir.x) * M_2_PI_INV - 0.5f;
	v = height * acos(ray->dir.y) * M_PI_INV - 0.5f;
	i = (v * width + u) % (width * height);
	return ((t_vec4){{\
(float)scene->skydome->pixels[i], \
(float)scene->skydome->pixels[++i], \
(float)scene->skydome->pixels[++i], \
(float)scene->skydome->pixels[++i]}});
}
