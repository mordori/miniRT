#include "objects.h"
#include "utils.h"

// Akenine-Möller & Trumbore
bool hit_triangle(const t_triangle* tri, const t_ray* ray, t_hit* hit) {
	t_vec3 e1 = vec3_sub(tri->v1, tri->v0);
	t_vec3 e2 = vec3_sub(tri->v2, tri->v0);
	t_vec3 pvec = vec3_cross(ray->dir, e2);
	float det = vec3_dot(e1, pvec);
	// if (fabsf(det) < M_EPSILON) // For transparent materials
	if (det < M_EPSILON)
		return false;

	float inv_det = 1.0f / det;
	t_vec3 tvec = vec3_sub(ray->origin, tri->v0);
	float u = vec3_dot(tvec, pvec) * inv_det;
	if (u < 0.0f || u > 1.0f)
		return false;

	t_vec3 qvec = vec3_cross(tvec, e1);
	float v = vec3_dot(ray->dir, qvec) * inv_det;
	if (v < 0.0f || u + v > 1.0f)
		return false;

	float t = vec3_dot(e2, qvec) * inv_det;
	if (t < G_EPSILON || t >= hit->t)
		return false;

	hit->t = t;
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	float w = 1.0f - u - v;

	t_vec3 normal;
	if (tri->has_normals) {
		t_vec3 n0 = vec3_scale(tri->n0, w);
		t_vec3 n1 = vec3_scale(tri->n1, u);
		t_vec3 n2 = vec3_scale(tri->n2, v);
		normal = vec3_normalize(vec3_add(vec3_add(n0, n1), n2));
	} else {
		normal = vec3_normalize(vec3_cross(e1, e2));
	}
	eval_hit_normal(ray, hit, normal);

	if (tri->has_uvs) {
		hit->uv.u = (tri->uv0.u * w) + (tri->uv1.u * u) + (tri->uv2.u * v);
		hit->uv.v = (tri->uv0.v * w) + (tri->uv1.v * u) + (tri->uv2.v * v);
	} else {
		hit->uv = (t_vec2){ 0 };
	}

	if (tri->has_uvs) {
		t_vec2 d_uv1 = vec2_sub(tri->uv1, tri->uv0);
		t_vec2 d_uv2 = vec2_sub(tri->uv2, tri->uv0);
		float f = 1.0f / (d_uv1.u * d_uv2.v - d_uv2.u * d_uv1.v);
		hit->tangent.x = f * (d_uv2.v * e1.x - d_uv1.v * e2.x);
		hit->tangent.y = f * (d_uv2.v * e1.y - d_uv1.v * e2.y);
		hit->tangent.z = f * (d_uv2.v * e1.z - d_uv1.v * e2.z);
		hit->tangent = vec3_normalize(hit->tangent);
	} else {
		hit->tangent = g_right;
	}
	hit->bitangent = vec3_cross(hit->normal, hit->tangent);
	return true;
}
