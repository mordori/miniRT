#include "objects.h"
#include "materials.h"

static inline float	solve_quadratic(const t_sphere *sphere, const t_ray *ray, float t_max);

t_parse_error	init_sphere(t_context *ctx, t_vec3 center, float diameter, t_material *mat)
{
	t_object	obj;
	float		radius;

	radius = diameter / 2.0f;
	obj = (t_object){0};
	obj.type = OBJ_SPHERE;
	obj.transform.pos = center;
	obj.shape.sphere.center = center;
	obj.shape.sphere.radius = radius;
	obj.shape.sphere.radius_squared = radius * radius;
	obj.material_id = new_material(ctx, mat);
	obj.flags |= OBJ_CAST_SHADOWS | OBJ_VISIBLE;
	return (add_object(ctx, &obj));
}

bool	hit_sphere(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	t_sphere	sphere;
	float		t;

	sphere = shape->sphere;
	if (sphere.radius < M_EPSILON)
		return (false);
	t = solve_quadratic(&sphere, ray, hit->t);
	if (t >= hit->t)
		return (false);
	hit->t = t;
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	hit->normal = vec3_div(vec3_sub(hit->point, sphere.center), sphere.radius);
	return (true);
}

static inline float	solve_quadratic(const t_sphere *sphere, const t_ray *ray, float t_max)
{
	t_vec3		oc;
	float		half_b;
	float		d;
	float		sqrt_d;
	float		root;

	oc = vec3_sub(ray->origin, sphere->center);
	half_b = vec3_dot(ray->dir, oc);
	d = half_b * half_b - (vec3_dot(oc, oc) - sphere->radius_squared);
	if (d < 0)
		return (M_INF);
	sqrt_d = sqrtf(d);
	root = -half_b - sqrt_d;
	if (root <= G_EPSILON || root >= t_max)
	{
		root = -half_b + sqrt_d;
		if (root <= G_EPSILON || root >= t_max)
			return (M_INF);
	}
	return (root);
}
