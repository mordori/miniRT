#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "materials.h"

void	init_directional_light(t_context *ctx, t_light *light)
{
	const float		distance = 999.0f;
	const float		angular_radius = 0.0046f;
	t_material		mat;
	t_object		obj;
	t_vec3			direction;
	float			radius;
	float			radiance;
	float			area;
	float			target_lux;

	direction = (t_vec3){{20.0f, 13.75f, 14.75f}};
	direction = vec3_normalize(direction);
	target_lux = 40.0f;
	area = 4.0f * M_PI * (angular_radius * angular_radius);
	radiance = target_lux / area / 25.0f;
	light->pos = vec3_scale(direction, distance);
	light->emission = vec3_scale((t_vec3){{1.0f, 1.0f, 1.0f}}, radiance);
	light->type = LIGHT_DIRECTIONAL;
	light->intensity = 1.0f;
	light->color = vec3_n(1.0f);
	obj = (t_object){0};
	radius = distance * angular_radius * 6.0f;
	obj.type = OBJ_SPHERE;
	obj.transform.pos = light->pos;
	obj.shape.sphere.center = obj.transform.pos;
	obj.shape.sphere.radius = radius;
	obj.shape.sphere.radius_squared = radius * radius;
	mat.emission = light->emission;
	mat.is_emissive = true;
	obj.material_id = new_material(ctx, &mat);
	obj.flags |=  OBJ_VISIBLE;
	add_object(ctx, &obj);
	light->obj = (t_object *)vector_getlast(&ctx->scene.objs);
}
