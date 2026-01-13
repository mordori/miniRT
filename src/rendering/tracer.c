#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"

static inline t_vec3	background_color(const t_texture *tex, const t_ray *ray);
static inline t_vec3	background_gradient(float t);
static inline bool	trace_ray(const t_scene *scene, t_ray *ray, t_path_data *data, uint32_t *seed);
static inline bool	scatter(t_ray *ray, t_hit *hit, uint32_t *seed);

t_vec3	trace_path(const t_scene *scene, const t_renderer *r, t_vec2 uv, uint32_t *seed)
{
	const t_viewport	*vp;
	t_ray				ray;
	t_vec3				pixel_loc;
	int					i;
	t_path_data			data;

	vp = &r->cam.viewport;
	pixel_loc = vec3_add(vp->pixel_00_loc, vec3_add(vec3_scale(vp->d_u, uv.u), vec3_scale(vp->d_v, uv.v)));
	ray = new_ray(r->cam.transform.pos, vec3_normalize(vec3_sub(pixel_loc, r->cam.transform.pos)));
	data.color = (t_vec3){0};
	data.throughput = (t_vec3){{1.0f, 1.0f, 1.0f}};
	data.mode = r->mode;
	i = 0;
	while (i < r->ray_bounces)
	{
		data.hit = (t_hit){0};
		data.hit.t = M_INF;
		if (!trace_ray(scene, &ray, &data, seed))
			break;
		++i;
	}
	return (data.color);
}

static inline bool	trace_ray(const t_scene *scene, t_ray *ray, t_path_data *data, uint32_t *seed)
{
	t_material		*mat;
	t_path_data		d;

	d = *data;
	if (hit_object(scene->selected_obj, ray, &d.hit) | hit_bvh(scene->bvh_root, ray, &d.hit))
	{
		mat = ((t_material **)scene->materials.items)[d.hit.obj->material_id];
		d.color = vec3_add(d.color, vec3_mul(d.throughput, compute_lighting(scene, &d.hit, 0, mat)));
		d.color = vec3_add(d.color, vec3_mul(d.throughput, mat->emission));
		if (d.mode == RENDER_PREVIEW)
			compute_ambient(scene, mat, &d.color);
		if (scatter(ray, &d.hit, seed))
		{
			d.throughput = vec3_mul(d.throughput, mat->albedo);
			*data = d;
			if (d.throughput.x < C_EPSILON || d.throughput.y < C_EPSILON || d.throughput.z < C_EPSILON)
				return (false);
			return (true);
		}
		*data = d;
		return (false);
	}
	d.color = vec3_add(d.color, vec3_mul(d.throughput, background_color(&scene->skydome, ray)));
	*data = d;
	return (false);
}

static inline bool	scatter(t_ray *ray, t_hit *hit, uint32_t *seed)
{
	t_vec3		origin;
	t_vec3		dir;
	t_vec3		random;

	random =  vec3_unit_random(seed);
	dir = vec3_add(hit->normal, random);
	origin = vec3_add(hit->point, vec3_scale(hit->normal, G_EPSILON));
	*ray = new_ray(origin, vec3_normalize(dir));
	return (true);
}

t_ray	new_ray(t_vec3 origin, t_vec3 dir)
{
	t_ray		ray;
	t_f_int		sign;

	ray.origin = origin;
	ray.dir = dir;
	ray.inv_dir.x = 1.0f / (ray.dir.x + copysignf(1e-20f, ray.dir.x));
	ray.inv_dir.y = 1.0f / (ray.dir.y + copysignf(1e-20f, ray.dir.y));
	ray.inv_dir.z = 1.0f / (ray.dir.z + copysignf(1e-20f, ray.dir.z));
	sign.f = ray.inv_dir.x;
	ray.sign[0] = sign.i >> 31;
	sign.f = ray.inv_dir.y;
	ray.sign[1] = sign.i >> 31;
	sign.f = ray.inv_dir.z;
	ray.sign[2] = sign.i >> 31;
	return (ray);
}

static inline t_vec3	background_color(const t_texture *tex, const t_ray *ray)
{
	t_float2	uv;
	t_int2		xy;
	uint32_t	i;
	t_vec3		result;
	const float	*pixels;

	if (!tex)
		return (background_gradient((ray->dir.y + 1.0f) * 0.5f));
	uv.u = (atan2f(ray->dir.z, ray->dir.x) + M_PI) * M_1_2PI;
	uv.v = acosf(ray->dir.y) * M_1_PI;
	xy.x = (uint32_t)(uv.u * (tex->width - 1));
	xy.y = (uint32_t)(uv.v * (tex->height - 1));
	i = (xy.y * tex->width + xy.x) * 4;
	pixels = (const float *)__builtin_assume_aligned(tex->pixels, 64);
	memcpy(&result, &pixels[i], sizeof(t_vec4));
	return (result);
}

static inline t_vec3	background_gradient(float t)
{
	t_vec3		result;
	uint32_t	c;

	c = lerp_color(RED, BLUE, t);
	result.r = (float)(((c >> 24) & 0xFF) * INV_255F);
	result.g = (float)(((c >> 16) & 0xFF) * INV_255F);
	result.b = (float)(((c >> 8) & 0xFF) * INV_255F);
	return (result);
}
