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
	mat->f0_dielectric = vec3_n(reflectance(mat->ior));
	*new_mat = *mat;
	vector_try_add(ctx, &ctx->scene.assets.materials, new_mat);
	return (ctx->scene.assets.materials.total - 1);
}

static t_vec3	apply_normal_map(const t_material *mat, t_vec3 n, t_path *path)
{
	t_vec3		sampled;

	sampled = sample_texture(&mat->normal_map, path->hit.uv);
	sampled = vec3_sub(vec3_scale(sampled, 2.0f), vec3_n(1.0f));
	sampled.x *= mat->bump_strength;
	sampled.y *= mat->bump_strength;
	sampled = vec3_normalize(sampled);
	n = vec3_normalize(mul_tbn(sampled, n, path->hit.tangent,
				path->hit.bitangent)); // Broken, until we have the proper transforms ready.
	return (n);
}

void	set_material_data(t_path *path)
{
	t_vec3		albedo;
	float		rough_bump;

	path->n = path->hit.normal;
	path->v = vec3_negate(path->ray.dir);
	if (path->mat->normal_map.pixels)
		path->n = apply_normal_map(path->mat, path->n, path);
	path->ndotv = clampf(vec3_dot(path->n, path->v), G_EPSILON, 1.0f);
	path->alpha = fmaxf(path->mat->roughness * path->mat->roughness, 0.001f);
	if (path->bounce > 0)
	{
		rough_bump = 0.05f * (float)path->bounce;
		path->alpha = fmaxf(path->alpha, rough_bump * rough_bump);
	}
	albedo = get_surface_color(path->mat, &path->hit);
	path->f0 = vec3_lerp(path->mat->f0_dielectric, albedo, path->mat->metallic);
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
