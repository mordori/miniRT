#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "utils.h"

static inline t_vec4	background_color(const t_texture *tex, const t_ray *ray);

t_vec4	trace_ray(const t_scene *scene, const t_camera *cam, uint32_t x, uint32_t y)
{
	const t_viewport	*vp;
	t_ray				ray;
	t_hit				hit;
	t_vec3				pixel_loc;
	t_vec3				offset;

	vp = &cam->viewport;
	offset = vec3_add(vec3_scale(vp->d_u, x), vec3_scale(vp->d_v, y));
	pixel_loc = vec3_add(vp->pixel_00_loc, offset);
	ray = new_ray(cam->transform.pos, pixel_loc);
	hit = (t_hit){0};
	hit.t = M_INF;
	if (hit_bvh(scene->bvh_root, &ray, &hit))
		return (calculate_lighting(scene, &hit, 0, ((t_material **)scene->materials.items)[hit.obj->material_id]));
	return (background_color(&scene->skydome, &ray));
}

t_ray	new_ray(t_vec3 origin, t_vec3 dest)
{
	t_ray		ray;
	t_f_int		sign;

	ray.origin = origin;
	ray.dir = vec3_normalize(vec3_sub(dest, ray.origin));
	ray.inv_dir.x = 1.0f / ray.dir.x + 1e-20f;
	ray.inv_dir.y = 1.0f / ray.dir.y + 1e-20f;
	ray.inv_dir.z = 1.0f / ray.dir.z + 1e-20f;
	sign.f = ray.inv_dir.x;
	ray.sign[0] = sign.i >> 31;
	sign.f = ray.inv_dir.y;
	ray.sign[1] = sign.i >> 31;
	sign.f = ray.inv_dir.z;
	ray.sign[2] = sign.i >> 31;
	return (ray);
}

static inline t_vec4	background_color(const t_texture *tex, const t_ray *ray)
{
	t_float2	uv;
	t_int2		xy;
	uint32_t	i;
	t_vec4		result;
	const float	*pixels;

	if (!tex)
		return ((t_vec4){{1.0f, 0.0f, 0.0f, 1.0f}});
	pixels = (const float *)__builtin_assume_aligned(tex->pixels, 64);
	uv.u = (atan2f(ray->dir.z, ray->dir.x) + M_PI) * M_1_2PI;
	uv.v = acosf(ray->dir.y) * M_1_PI;
	xy.x = (uint32_t)(uv.u * (tex->width - 1));
	xy.y = (uint32_t)(uv.v * (tex->height - 1));
	i = (xy.y * tex->width + xy.x) * 4;
	memcpy(&result, &pixels[i], sizeof(t_vec4));
	result.a = 1.0f;
	return (result);
}
