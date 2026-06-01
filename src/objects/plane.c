#include "defines.h"
#include "objects.h"
#include "scene.h"
#include "utils.h"

t_error init_plane(t_context* ctx, t_vec3 point, t_vec3 normal, uint32_t mat_id) {
	t_object* obj = malloc(sizeof(t_object));
	if (!obj)
		return E_MALLOC;

	*obj = (t_object){ 0 };
	obj->type = OBJ_PLANE;
	obj->transform.pos = point;
	obj->transform.rot = quat_from_dir(normal);
	obj->transform.scale = g_one;
	update_transform(&obj->transform);
	update_bounds(obj);

	onb(g_up, &obj->shape.plane.u, &obj->shape.plane.v);
	obj->shape.plane.point = g_zero;
	obj->shape.plane.normal = g_up;
	obj->shape.plane.d = 0.0f;
	obj->material_id = mat_id;
	obj->mat = ((t_material**)ctx->scene.assets.materials.items)[mat_id];
	obj->flags = obj->mat->flags;
	if (!vector_add(&ctx->scene.geo.planes, obj)) {
		free(obj);
		return E_MALLOC;
	}
	return E_OK;
}

bool hit_planes(const t_context* ctx, const t_ray* ray, t_hit* hit) {
	bool res = false;
	size_t idx = ctx->scene.geo.planes.total;
	while (idx--) {
		t_object* obj = ((t_object**)ctx->scene.geo.planes.items)[idx];
		t_ray r = ray_world_to_object(ray, &obj->transform.world_to_object);

		bool isHiddenScene = (obj->flags & OBJ_HIDDEN_SCENE);
		bool isHiddenCam = (hit->is_primary && (obj->flags & OBJ_HIDDEN_CAM));
		bool hitPlane = hit_plane(&obj->shape, &r, hit);

		if (!isHiddenScene && !isHiddenCam && hitPlane) {
			hit->obj = obj;
			hit_object_to_world(hit, &obj->transform);
			res = true;
		}
	}
	return res;
}

bool hit_plane(const t_shape* shape, const t_ray* ray, t_hit* hit) {
	const t_plane* plane = &shape->plane;

	float denom = ray->dir.y;
	if (fabsf(denom) < G_EPSILON)
		return false;

	float t = -ray->origin.y / denom;
	if (t < G_EPSILON || t >= hit->t)
		return false;

	hit->t = t;
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	hit->normal = g_up;
	if (denom > 0)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	hit->uv.u = vec3_dot(hit->point, plane->u);
	hit->uv.v = vec3_dot(hit->point, plane->v);
	hit->tangent = plane->u;
	hit->bitangent = plane->v;
	return true;
}
