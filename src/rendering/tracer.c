#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "libft_random.h"

static inline t_vec3	background_color(const t_texture *tex, const t_ray *ray);
static inline t_vec3	background_gradient(float t);
static inline t_path_data	trace_ray(const t_scene *scene, t_ray *ray, t_path_data *data, uint32_t *seed);
static inline bool	scatter(t_ray *ray, t_hit *hit, uint32_t *seed);

t_vec3	trace_path(const t_scene *scene, const t_renderer *r, t_vec2 uv, uint32_t *seed)
{
	const t_viewport	*vp;
	t_ray				ray;
	t_vec3				pixel_loc;
	t_path_data			data;
	int32_t				previous_bounce;

	vp = &r->cam.viewport;
	pixel_loc = vec3_add(vp->pixel_00_loc, vec3_add(vec3_scale(vp->d_u, uv.u), vec3_scale(vp->d_v, uv.v)));
	ray = new_ray(r->cam.transform.pos, vec3_normalize(vec3_sub(pixel_loc, r->cam.transform.pos)));
	data = (t_path_data){0};
	data.throughput = (t_vec3){{1.0f, 1.0f, 1.0f}};
	data.mode = r->mode;
	while (data.bounce < r->ray_bounces)
	{
		data.hit = (t_hit){0};
		data.hit.t = M_INF;
		previous_bounce = data.bounce;
		data = trace_ray(scene, &ray, &data, seed);
		if (previous_bounce == data.bounce)
			break;
	}
	return (data.color);
}

static inline t_path_data	trace_ray(const t_scene *scene, t_ray *ray, t_path_data *data, uint32_t *seed)
{
	t_material		*mat;
	t_path_data		d;
	float			p;

	d = *data;
	if (hit_object(scene->selected_obj, ray, &d.hit) | hit_bvh(scene->bvh_root, ray, &d.hit, 0))
	{
		mat = ((t_material **)scene->materials.items)[d.hit.obj->material_id];
		if (mat->is_emissive)
		{
			if (d.bounce == 0)
				d.color = vec3_add(d.color, vec3_mul(d.throughput, mat->emission));
			return (d);
		}
		// d.color = vec3_add(d.color, vec3_mul(d.throughput, compute_directional(scene, &d.hit, mat)));
		if (d.mode == RENDER_PREVIEW && d.bounce == 0)
			d.color = vec3_add(d.color, vec3_mul(d.throughput, compute_ambient(scene, mat)));
		if (d.mode == RENDER_PREVIEW)
			return (d);
		d.color = vec3_add(d.color, vec3_mul(d.throughput, compute_lighting(scene, &d.hit, mat, seed)));
		if (d.mode == RENDER_REFINE && scatter(ray, &d.hit, seed))
		{
			d.throughput = vec3_mul(d.throughput, mat->albedo);
			if (d.bounce > 2)
			{
				p = fmaxf(d.throughput.r, fmaxf(d.throughput.g, d.throughput.b));
				if (p > 0.95f)
					p = 0.95f;
				else if (p < 0.05f)
					p = 0.05f;
				if (randomf01(seed) > p)
					return (d);
				d.throughput = vec3_scale(d.throughput, 1.0f / p);
			}
			++d.bounce;
			return (d);
		};
		return (d);
	}
	// if (d.bounce == 0)
		// d.color = vec3_add(d.color, vec3_mul(d.throughput, vec3_div(background_color(&scene->skydome, ray), 0.3f)));
	// else
	d.color = vec3_add(d.color, vec3_mul(d.throughput, background_color(&scene->skydome, ray)));
	return (d);
}

static inline bool	scatter(t_ray *ray, t_hit *hit, uint32_t *seed)
{
	t_vec3		origin;
	t_vec3		dir;
	t_vec3		random;
	float		len_sq;

	random =  vec3_unit_random(seed);
	dir = vec3_add(hit->normal, random);
	len_sq = vec3_dot(dir, dir);
	if (len_sq < LEN_SQ_EPSILON)
	{
		dir = hit->normal;
		len_sq = 1.0f;
	}
	origin = vec3_add(hit->point, vec3_scale(hit->normal, G_EPSILON));
	*ray = new_ray(origin, vec3_div(dir, sqrtf(len_sq)));
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

	if (tex)
		return (background_gradient((ray->dir.y + 1.0f) * 0.5f));
	uv.u = ft_clamp01((atan2f(ray->dir.z, ray->dir.x) + M_PI) * M_1_2PI);
	uv.v = ft_clamp01(acosf(ray->dir.y) * M_1_PI);
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
