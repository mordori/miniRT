// #include "rendering.h"
// #include "utils.h"
// #include "scene.h"

// t_vec3	compute_lighting(const t_context *ctx, const t_path *path, const t_light *light, t_pixel *pixel)
// {
// 	t_vec3		color;
// 	t_vec3		hit_to_light;
// 	t_vec3		orig_biased;
// 	t_vec3		pos;
// 	t_vec3		dir;
// 	t_vec2		uv;
// 	float		dist;
// 	float		ndotl;
// 	float		cos_light;
// 	float		area;

// 	color = (t_vec3){0};
// 	pos = light->pos;
// 	orig_biased = vec3_add(path->hit.point, vec3_scale(path->hit.normal, G_EPSILON));
// 	hit_to_light = vec3_sub(pos, orig_biased);
// 	dist = vec3_length(hit_to_light);
// 	if (dist < LEN_EPSILON)
// 		return (color);
// 	dir = vec3_scale(hit_to_light, 1.0f / dist);
// 	ndotl = vec3_dot(path->hit.normal, dir);
// 	if (ndotl <= 0.0f)
// 		return (color);
// 	if (hit_shadow(&ctx->scene, orig_biased, dir, dist - G_EPSILON))
// 		return (color);
// 	color = add_light(light->emission, path->mat->albedo, ndotl, dist);
// 	color = vec3_scale(color, (float)ctx->scene.lights.total);
// 	return (color);
// }

// static inline t_vec3	direct_lighting_editing(const t_context *ctx, t_path *path, const t_light *light)
// {
// 	t_vec3		res;
// 	t_vec3		hit_biased;
// 	t_vec3		hit_to_light_center;
// 	float		dist;
// 	float		t_ca;
// 	float		ca_dist_sq;
// 	float		t_hc;
// 	float		weight;

// 	hit_biased = vec3_bias(path->hit.point, path->n);
// 	path->l = vec3_normalize(vec3_sub(light->pos, hit_biased));
// 	if (vec3_dot(path->l, path->l) < LEN_SQ_EPSILON)
// 		return (vec3_n(0.0f));
// 	set_shader_data(path);
// 	if (path->ndotl <= G_EPSILON)
// 		return (vec3_n(0.0f));
// 	t_ca = vec3_dot(hit_to_light_center, path->l);
// 	ca_dist_sq = vec3_dot(hit_to_light_center, hit_to_light_center) - t_ca * t_ca;
// 	t_hc = sqrtf(fmaxf(0.0f, light->obj->shape.sphere.radius_sq - ca_dist_sq));
// 	dist = t_ca - t_hc;
// 	if (!(path->mat->flags & MAT_NO_REC_SHADOW) && hit_shadow(&ctx->scene, hit_biased, path->l, dist - B_EPSILON))
// 		return (vec3_n(0.0f));
// 	weight = power_heuristic(path->pdf, bsdf_pdf(path));
// 	res = vec3_mul(light->mat->emission, bsdf(path));
// 	res = vec3_scale(res, (path->ndotl / path->pdf) *  weight);
// 	return (res);
// }

// t_vec3	compute_ambient(const t_scene *scene, const t_material *mat)
// {
// 	t_light		*light;
// 	t_vec3		color;

// 	light = (t_light *)&scene->ambient_light;
// 	color = vec3_mul(light->color, mat->albedo);
// 	color = vec3_scale(color, light->intensity);
// 	return (color);
// }

// t_vec3	compute_directional(const t_scene *scene, const t_hit *hit, const t_material *mat)
// {
// 	t_light		*light;
// 	t_vec3		color;
// 	t_vec3		dir;
// 	float		ndotl;
// 	t_vec3		orig_biased;

// 	light = (t_light *)&scene->directional_light;
// 	orig_biased = vec3_add(hit->point, vec3_scale(hit->normal, B_EPSILON));
// 	dir = vec3_normalize(light->dir);
// 	ndotl = vec3_dot(hit->normal, dir);
// 	if (ndotl <= 0.0f || hit_shadow(scene, orig_biased, dir, M_INF))
// 		return ((t_vec3){0});
// 	color = vec3_mul(light->color, mat->albedo);
// 	color = vec3_scale(color, light->intensity * ndotl);
// 	return (color);
// }

// static inline t_vec3	add_light(const t_vec3 emission, const t_vec3 albedo, float ndotl, float dist)
// {
// 	t_vec3		result;
// 	float		attenuation;

// 	attenuation = ndotl / fmaxf(dist * dist, 0.1f);
// 	result = vec3_mul(emission, albedo);
// 	result = vec3_scale(result, attenuation);
// 	return (result);
// }

// static inline bool	hit_shadow(const t_scene *scene, t_vec3 orig, t_vec3 dir, float dist)
// {
// 	t_ray		ray;

// 	ray = new_ray(orig, dir);
// 	return (hit_bvh_shadow(scene->bvh_root, &ray, dist));
// }
