#include "materials.h"
#include "rendering.h"

static inline float d_ggx(float ndoth, float a);
static inline float v_smith_ggx(float ndotv, float ndotl, float alpha);
static inline float burley(const t_path* path);

float reflectance(float ior) {
	float reflectance = (ior - 1.0f) / (ior + 1.0f);
	reflectance *= reflectance;
	return reflectance;
}

float pdf_ggx_vndf(const t_path* path) {
	float a2 = path->alpha * path->alpha;
	float d = d_ggx(path->ndoth, path->alpha);
	float den = path->ndotv + sqrtf(a2 + (1.0f - a2) * (path->ndotv * path->ndotv));
	return d / fmaxf(2.0f * den, G_EPSILON);
}

t_vec3 cook_torrance_brdf(const t_path* path) {
	float d = d_ggx(path->ndoth, path->alpha);
	float v = v_smith_ggx(path->ndotv, path->ndotl, path->alpha);
	t_vec3 f = vec3_schlick(path->f0, path->vdoth);
	return vec3_scale(f, d * v);
}

// Trowbridge-Reitz
static inline float d_ggx(float ndoth, float a) {
	float a2 = a * a;
	float den = (ndoth * ndoth) * (a2 - 1.0f) + 1.0f;
	den = M_PI * (den * den);
	return a2 / den;
}

static inline float v_smith_ggx(float ndotv, float ndotl, float a) {
	float a2 = a * a;
	float ggx_v = ndotl * sqrtf((ndotv * ndotv) * (1.0f - a2) + a2);
	float ggx_l = ndotv * sqrtf((ndotl * ndotl) * (1.0f - a2) + a2);
	return 0.5f / fmaxf((ggx_v + ggx_l), G_EPSILON);
}

float pdf_cos(float ndotl) {
	return ndotl * M_1_PI;
}

t_vec3 disney_brdf(const t_path* path) {
	t_vec3 f = vec3_schlick(path->f0, path->ldoth);
	t_vec3 k_d = vec3_sub(vec3_n(1.0f), f);
	t_vec3 f_d = vec3_mul(get_surface_color(path->mat, &path->hit), k_d);
	float weight = (1.0f - path->mat->metallic) * burley(path);
	return vec3_scale(f_d, weight);
}

static inline float burley(const t_path* path) {
	float f90 = 0.5f + 2.0f * path->mat->roughness * path->ldoth * path->ldoth;
	float l_scatter = f_schlick(path->ndotl, 1.0f, f90);
	float v_scatter = f_schlick(path->ndotv, 1.0f, f90);
	return l_scatter * v_scatter * M_1_PI;
}
