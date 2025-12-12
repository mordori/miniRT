#include "objects.h"

static inline float	solve_quadratic(const t_sphere *sphere, const t_ray *ray, float t_max);

t_shape	init_sphere(const t_object *obj, char **params)
{
	t_sphere	sphere;
	t_shape		shape;

	// For testing rendering
	// -----------------------
		(void)params;
		sphere.radius = 0.5f;
	// -----------------------

	sphere.radius_squared = sphere.radius * sphere.radius;
	sphere.center = obj->transform.pos;
	shape.sphere = sphere;
	return (shape);
}

bool	hit_sphere(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	t_sphere	sphere;
	float		t;

	sphere = shape->sphere;
	if (sphere.radius < 1e-6f)
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
	t_vec3	oc;
	float	half_b;
	float	d;
	float	sqrt_d;
	float	root;

	oc = vec3_sub(ray->origin, sphere->center);
	half_b = vec3_dot(ray->dir, oc);
	d = half_b * half_b - (vec3_dot(oc, oc) - sphere->radius_squared);
	if (d < 0)
		return (M_INF);
	sqrt_d = sqrtf(d);
	root = -half_b - sqrt_d;
	if (root <= 0.001f || root >= t_max)
	{
		root = -half_b + sqrt_d;
		if (root <= 0.001f || root >= t_max)
			return (M_INF);
	}
	return (root);
}
