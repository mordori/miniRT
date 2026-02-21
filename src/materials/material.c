#include "materials.h"
#include "rendering.h"
#include "utils.h"

uint32_t	new_material(t_context *ctx, t_material *mat)
{
	t_material		*new_mat;

	new_mat = malloc(sizeof(*new_mat));
	if (!new_mat)
		fatal_error(ctx, errors(ERR_MATADD), __FILE__, __LINE__);
	mat->ior = fmaxf(mat->ior, 1.0f);
	*new_mat = *mat;
	vector_try_add(ctx, &ctx->scene.assets.materials, new_mat);
	return (ctx->scene.assets.materials.total - 1);
}

void	set_material_data(t_path *path)
{
	t_vec3		albedo;
	t_vec3		f0_dielectric;
	float		rough_bump;

	path->n = path->hit.normal;
	path->v = vec3_negate(path->ray.dir);
	path->ndotv = clampf(vec3_dot(path->n, path->v), G_EPSILON, 1.0f);
	path->alpha = fmaxf(path->mat->roughness * path->mat->roughness, 0.001f);
	if (path->bounce > 0)
	{
		rough_bump = 0.05f * (float)path->bounce;
		path->alpha = fmaxf(path->alpha, rough_bump * rough_bump);
	}
	f0_dielectric = vec3_n(reflectance(path->mat->ior));
	albedo = get_surface_color(path->mat, &path->hit);
	path->f0 = vec3_lerp(f0_dielectric, albedo, path->mat->metallic);
}

void	set_shader_data(t_path *path)
{
	t_vec3		h;
	float		len_sq;

	h = vec3_add(path->v, path->l);
	len_sq = vec3_dot(h, h);
	if (len_sq < LEN_SQ_EPSILON)
		path->h = path->n;
	else
		path->h = vec3_scale(h, 1.0f / sqrtf(len_sq));
	path->ndotl = clampf01(vec3_dot(path->n, path->l));
	path->ndoth = clampf01(vec3_dot(path->n, path->h));
	path->vdoth = clampf01(vec3_dot(path->v, path->h));
	path->ldoth = clampf01(vec3_dot(path->l, path->h));
}
