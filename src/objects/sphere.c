#include "objects.h"

static inline float	solve_quadratic(t_sphere *sphere, t_ray *ray, t_hit *hit);

t_shape	init_sphere(char **params)
{
	t_sphere	sphere;
	t_shape		shape;

	// For testing rendering
	// -----------------------
		(void)params;
		sphere.radius = 1.0f;
	// -----------------------

	sphere.radius_squared = sphere.radius * sphere.radius;
	shape.sphere = sphere;
	return (shape);
}

bool	hit_sphere(t_sphere *sphere, t_ray *ray, t_hit *hit)
{
	float	t;

	t = solve_quadratic(sphere, ray, hit);
	if (t == M_INF)
		return (false);
	hit->t = t;
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	hit->normal = vec3_normalize(vec3_sub(hit->point, sphere->center));
	return (true);
}

static inline float	solve_quadratic(t_sphere *sphere, t_ray *ray, t_hit *hit)
{
	t_vec3	oc;
	float	a;
	float	b;
	float	c;
	float	discriminant;

	oc = vec3_sub(sphere->center, ray->origin);
	a = vec3_dot(ray->dir, ray->dir);
	b = -2.0f * vec3_dot(ray->dir, sphere->center);
	c = vec3_dot(oc, oc) - sphere->radius_squared;
	discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0)
		return (M_INF);
	return ((-b - sqrtf(discriminant)) / (2.0f * a));
}
