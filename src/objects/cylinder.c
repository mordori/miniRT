#include "objects.h"
#include "materials.h"
#include <math.h>

t_error	init_cylinder(t_context *ctx, t_cylinder *cy, t_material *mat)
{
	t_object	obj;

	obj = (t_object){0};
	obj.type = OBJ_CYLINDER;
	obj.transform.pos = cy->center;
	obj.shape.cylinder = *cy;
	obj.material_id = new_material(ctx, mat);
	obj.flags = OBJ_VISIBLE | OBJ_CAST_SHADOWS;
	return (add_object(ctx, &obj));
}

static bool	solve_quadratic(const t_cylinder *cyl, const t_ray *ray,
				const t_vec3 oc, float t[2])
{
	t_vec3	d_perp;
	t_vec3	oc_perp;
	float	coef[3];
	float	disc;

	d_perp = vec3_sub(ray->dir, vec3_scale(cyl->axis,
				vec3_dot(ray->dir, cyl->axis)));
	oc_perp = vec3_sub(oc, vec3_scale(cyl->axis, vec3_dot(oc, cyl->axis)));
	coef[0] = vec3_dot(d_perp, d_perp);
	if (coef[0] < G_EPSILON)
		return (false);
	coef[1] = vec3_dot(oc_perp, d_perp);
	coef[2] = vec3_dot(oc_perp, oc_perp) - cyl->radius * cyl->radius;
	disc = coef[1] * coef[1] - coef[0] * coef[2];
	if (disc < 0.0f)
		return (false);
	disc = sqrtf(disc);
	t[0] = (-coef[1] - disc) / coef[0];
	t[1] = (-coef[1] + disc) / coef[0];
	return (true);
}

static float	get_projection(const t_cylinder *cyl, const t_vec3 oc,
					const t_vec3 dir, float t)
{
	return (vec3_dot(vec3_add(oc, vec3_scale(dir, t)), cyl->axis));
}

static bool	hit_body(const t_cylinder *cyl, const t_ray *ray, t_hit *hit)
{
	t_vec3	oc;
	float	half_h;
	float	t[2];
	float	proj;

	oc = vec3_sub(ray->origin, cyl->center);
	half_h = cyl->height * 0.5f;
	if (!solve_quadratic(cyl, ray, oc, t))
		return (false);
	if (t[0] > G_EPSILON && t[0] < hit->t)
	{
		proj = get_projection(cyl, oc, ray->dir, t[0]);
		if (proj >= -half_h && proj <= half_h)
			return (hit->t = t[0], true);
	}
	if (t[1] > G_EPSILON && t[1] < hit->t)
	{
		proj = get_projection(cyl, oc, ray->dir, t[1]);
		if (proj >= -half_h && proj <= half_h)
			return (hit->t = t[1], true);
	}
	return (false);
}

static void	set_body_normal(const t_cylinder *cyl, const t_ray *ray, t_hit *hit)
{
	t_vec3	hit_vec;
	float	proj;

	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, hit->t));
	hit_vec = vec3_sub(hit->point, cyl->center);
	proj = vec3_dot(hit_vec, cyl->axis);
	hit->normal = vec3_normalize(vec3_sub(hit->point,
				vec3_add(cyl->center, vec3_scale(cyl->axis, proj))));
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
}

static bool	hit_cap(const t_cylinder *cyl, const t_ray *ray,
				float offset, t_hit *hit)
{
	t_vec3	cap_ctr;
	float	denom;
	float	t;
	t_vec3	to_p;

	cap_ctr = vec3_add(cyl->center, vec3_scale(cyl->axis, offset));
	denom = vec3_dot(ray->dir, cyl->axis);
	if (fabsf(denom) < G_EPSILON)
		return (false);
	t = vec3_dot(vec3_sub(cap_ctr, ray->origin), cyl->axis) / denom;
	if (t < G_EPSILON || t >= hit->t)
		return (false);
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	to_p = vec3_sub(hit->point, cap_ctr);
	if (vec3_dot(to_p, to_p) > cyl->radius * cyl->radius)
		return (false);
	hit->t = t;
	if (offset > 0.0f)
		hit->normal = cyl->axis;
	else
		hit->normal = vec3_scale(cyl->axis, -1.0f);
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	return (true);
}

bool	hit_cylinder(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	const t_cylinder	*cyl;
	float				half_h;
	t_hit				tmp;
	bool				found;

	cyl = &shape->cylinder;
	half_h = cyl->height * 0.5f;
	if (cyl->radius < G_EPSILON || cyl->height < G_EPSILON)
		return (false);
	found = false;
	tmp = *hit;
	if (hit_body(cyl, ray, &tmp))
	{
		set_body_normal(cyl, ray, &tmp);
		*hit = tmp;
		found = true;
	}
	if (hit_cap(cyl, ray, half_h, &tmp) && tmp.t < hit->t)
	{
		*hit = tmp;
		found = true;
	}
	if (hit_cap(cyl, ray, -half_h, &tmp) && tmp.t < hit->t)
	{
		*hit = tmp;
		found = true;
	}
	return (found);
}

t_vec3	random_point_on_cylinder(const t_shape *shape, float u, float v)
{
	t_vec3	result;

	(void)shape;
	(void)u;
	(void)v;
	result = vec3_n(1.0f);
	return (result);
}

t_vec3	normal_at_cylinder(const t_shape *shape, const t_vec3 pos)
{
	t_vec3	result;

	(void)shape;
	(void)pos;
	result = vec3_n(1.0f);
	return (result);
}
