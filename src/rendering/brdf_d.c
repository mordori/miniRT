#include "materials.h"
#include "rendering.h"

static inline float burley(const t_path* path);

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
