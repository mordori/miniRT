#include "objects.h"
#include "utils.h"

static inline float solve_quadratic(const t_sphere* sphere, const t_ray* ray, float t_max);

t_error init_sphere(t_context* ctx, t_vec3 center, float diameter, uint32_t mat_id) {
	t_object obj = (t_object){ 0 };
	obj.type = OBJ_SPHERE;
	obj.material_id = mat_id;
	obj.transform.pos = center;
	float radius = diameter * 0.5f;
	obj.shape.sphere.radius = radius;
	obj.shape.sphere.radius_sq = radius * radius;
	obj.transform.rot.w = 1.0f;
	return add_object(ctx, &obj);
}

bool hit_sphere(const t_shape* shape, const t_ray* ray, t_hit* hit) {
	t_sphere sphere = shape->sphere;
	if (sphere.radius < M_EPSILON)
		return false;

	float t = solve_quadratic(&sphere, ray, hit->t);
	if (t >= hit->t)
		return false;

	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	t_vec3 normal_outward = vec3_normalize(hit->point);
	hit->point = vec3_scale(normal_outward, sphere.radius);
	eval_hit_normal(ray, hit, normal_outward);
	hit->uv = spherical_uv(normal_outward);

	hit->tangent = vec3(-normal_outward.z, 0.0f, normal_outward.x);
	float len_sq = vec3_dot(hit->tangent, hit->tangent);
	if (len_sq < G_EPSILON)
		hit->tangent = g_right;
	else
		hit->tangent = vec3_scale(hit->tangent, 1.0f / sqrtf(len_sq));
	hit->bitangent = vec3_cross(hit->normal, hit->tangent);
	hit->t = t;
	return true;
}

static inline float solve_quadratic(const t_sphere* sphere, const t_ray* ray, float t_max) {
	float a = vec3_dot(ray->dir, ray->dir);
	float half_b = vec3_dot(ray->dir, ray->origin);
	float d = half_b * half_b - a * (vec3_dot(ray->origin, ray->origin) - sphere->radius_sq);
	if (d < 0)
		return M_INF;

	float sqrt_d = sqrtf(d);
	a = 1.0f / a;
	float root = (-half_b - sqrt_d) * a;
	if (root <= G_EPSILON || root >= t_max) {
		root = (-half_b + sqrt_d) * a;
		if (root <= G_EPSILON || root >= t_max)
			return M_INF;
	}
	return root;
}
