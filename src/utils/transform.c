#include "utils.h"

void	update_transform(t_transform *t)
{
	t_mat4		translation;
	t_mat4		rotation;
	t_mat4		scale;

	translation = mat4_translate(t->pos);
	rotation = mat4_identity();
	scale = mat4_scale(t->scale);
	t->object_to_world = mat4_mul(&translation, &rotation);
	t->object_to_world = mat4_mul(&t->object_to_world, &scale);
	mat4_inverse(&t->object_to_world, &t->world_to_object);
}
