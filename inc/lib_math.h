#ifndef LIB_MATH_H
#define LIB_MATH_H

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <xmmintrin.h>

#pragma region defines
#ifndef M_PIf
#define M_PIf 3.14159265358979323846f
#endif

#ifndef M_PI_2f
#define M_PI_2f 1.57079632679489661923f
#endif

#ifndef M_PI_4f
#define M_PI_4f 0.78539816339744830962f
#endif

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154f
#endif

#ifndef M_1_2PIf
#define M_1_2PIf 0.15915494309189533577f
#endif

#ifndef M_TAUf
#define M_TAUf 6.28318530717958647693f
#endif

#ifndef M_INFf
#define M_INFf FLT_MAX
#endif

#define M_1_255f 0.003921568627451f
#define M_1_2P2f 0.454545454545454f

#define M_EPSILON 1e-6f
#define LEN_EPSILON 1e-6f
#define LEN_SQ_EPSILON 1e-12f
#define B_EPSILON 1e-4f
#define G_EPSILON 1e-5f

#define GOLDEN_RATIO 0.6180339887498948f
#define PLASTIC_CONSTANT 0.7548776662466927f
#define PLASTIC_CONSTANT2 0.5698402909980532f

#define ONE_MINUS_GR 0.3819660112501052f
#define ONE_MINUS_PC 0.2451223337533073f
#define ONE_MINUS_PC2 0.4301597090019468f
#pragma endregion

#pragma region typedefs
typedef struct s_vec2i t_vec2i;
typedef struct s_vec2i t_int2;
typedef struct s_vec2ui t_uint2;
typedef struct s_mat2 t_mat2;
typedef struct s_mat3 t_mat3;
typedef struct s_mat4 t_mat4;

typedef union u_f_ui t_f_ui;
typedef union u_v4ui_ui t_v4ui_ui;
typedef union u_vec2 t_point;
typedef union u_vec2 t_float2;
typedef union u_vec2 t_vec2;
typedef union u_vec3 t_vec3;
typedef union u_vec4 t_vec4;
typedef union u_vec3 t_float3;
typedef union u_vec4 t_float4;
typedef union u_color t_color;
typedef union u_vec4 t_quat;

typedef __attribute__((vector_size(16))) float t_v4sf;
typedef __attribute__((vector_size(16))) int32_t t_v4si;
typedef __attribute__((vector_size(16))) uint32_t t_v4ui;
#pragma endregion

#pragma region structs and unions
struct __attribute__((aligned(16))) s_mat2 {
	float m[2][2];
};

struct __attribute__((aligned(16))) s_mat3 {
	float m[3][3];
};

struct __attribute__((aligned(16))) s_mat4 {
	union {
		t_v4sf rows[4];
		float m[4][4];
	};
};

union u_v4ui_ui {
	t_v4ui v;
	uint32_t s[4];
};

union u_f_ui {
	float f;
	uint32_t i;
};

struct s_vec2i {
	int32_t x, y;
};

struct s_vec2ui {
	uint32_t x, y;
};

union u_vec2 {
	struct {
		float x, y;
	};
	struct {
		float u, v;
	};
	struct {
		float sin, cos;
	};
	struct {
		float width, height;
	};
};

union __attribute__((aligned(16))) u_vec3 {
	t_v4sf v;
	float data[4];
	struct {
		float x, y, z, w;
	};
	struct {
		float r, g, b, a;
	};
	struct {
		float right, up, forward;
	};
	struct {
		float angle, sin, cos;
	};
	struct {
		float width, height, depth;
	};
	t_vec2 xy, uv;
};

union __attribute__((aligned(16))) u_vec4 {
	t_v4sf v;
	float data[4];
	struct {
		float x, y, z, w;
	};
	struct {
		float r, g, b, a;
	};
	t_vec3 xyz, rgb, uvw;
};

union u_color {
	uint32_t full;
	struct {
		uint8_t r, g, b, a;
	};
};
#pragma endregion

#pragma region forward declarations
static inline int ft_imax(int a, int b);
static inline int ft_imin(int a, int b);
static inline float ft_lerp(float a, float b, float t);
static inline float ft_lerp_fast(float a, float b, float t);
static inline float ft_normalize(float value, float min, float max);
static inline float clampf01(float value);
static inline float clampfn11(float value);
static inline float clampf(float value, float min, float max);
static inline float f_schlick(float u, float f0, float f90);
static inline float degrees_to_rad(float degrees);
static inline float rad_to_degrees(float rad);
static inline bool is_pot(size_t n);
static inline size_t ft_pow(size_t n, size_t e);
static inline uint32_t ft_uint_min(uint32_t a, uint32_t b);
static inline bool is_nan_inf(float x);

// mat3
// -----------------------------------------------------------------
static inline float mat3_det(const t_mat3* src);

// mat4
// -----------------------------------------------------------------
static inline bool mat4_inverse(const t_mat4* src, t_mat4* out);
static inline t_mat3 mat4_sub(const t_mat4* src, int skip_r, int skip_c);
static inline float mat4_cofactor(const t_mat4* src, int row, int col);
static inline float mat4_det(const t_mat4* src);
static inline t_mat4 mat4_identity(void);
static inline t_mat4 mat4_transpose(const t_mat4* m);
static inline t_mat4 mat4_mul(const t_mat4* a, const t_mat4* b);
static inline t_vec4 mat4_mul_vec4(const t_mat4* m, t_vec4 v);
static inline t_mat4 mat4_translate(t_vec3 t);
static inline t_mat4 mat4_scale(t_vec3 s);
static inline t_mat4 mat4_rot_x(float rad);
static inline t_mat4 mat4_rot_y(float rad);
static inline t_mat4 mat4_rot_z(float rad);
static inline t_vec3 mat4_mul_vec3(const t_mat4* m, t_vec3 v);
static inline t_mat4 mat4_rot(t_vec3 rot);

// vec2i
// -----------------------------------------------------------------
static inline t_vec2i vec2i(int32_t x, int32_t y);
static inline t_vec2i vec2i_n(int32_t n);
static inline t_vec2i vec2i_f(float x, float y);
static inline t_vec2i vec2i_nf(float n);
static inline t_vec2i vec2i_add(t_vec2i a, t_vec2i b);
static inline t_vec2i vec2i_sub(t_vec2i a, t_vec2i b);
static inline t_vec2i vec2i_scale(t_vec2i vec, float s);

// vec2
// -----------------------------------------------------------------
static inline t_vec2 vec2(float x, float y);
static inline t_vec2 vec2_n(float n);
static inline t_vec2 vec2_add(t_vec2 a, t_vec2 b);
static inline t_vec2 vec2_sub(t_vec2 a, t_vec2 b);
static inline t_vec2 vec2_scale(t_vec2 vec, float s);

// vec3
// -----------------------------------------------------------------
static inline t_vec3 vec3(float x, float y, float z);
static inline t_vec3 vec3_n(float n);
static inline t_vec3 vec3_add(t_vec3 a, t_vec3 b);
static inline t_vec3 vec3_sub(t_vec3 a, t_vec3 b);
static inline t_vec3 vec3_scale(t_vec3 vec, float s);
static inline t_vec3 vec3_cross(t_vec3 a, t_vec3 b);
static inline float vec3_dot(t_vec3 a, t_vec3 b);
static inline t_vec3 vec3_div(t_vec3 vec, float div);
static inline float vec3_length(t_vec3 vec);
static inline t_vec3 vec3_normalize(t_vec3 vec);
static inline t_vec3 vec3_reflect(t_vec3 vec, t_vec3 n);
static inline t_vec3 vec3_unit_random(uint32_t* seed);
static inline t_vec3 vec3_min(t_vec3 vec, float min);
static inline t_vec3 vec3_max(t_vec3 vec, float max);
static inline t_vec3 vec3_sqrt(t_vec3 vec);
static inline t_vec3 vec3_clamp01(t_vec3 vec);
static inline t_vec3 vec3_clamp_mag(t_vec3 vec, float max);
static inline t_vec3 vec3_add_n(t_vec3 vec, float n);
static inline t_vec3 vec3_mul(t_vec3 a, t_vec3 b);
static inline t_vec3 vec3_negate(t_vec3 vec);
static inline t_vec3 vec3_clamp(t_vec3 vec, float min, float max);
static inline t_vec3 vec3_lerp(t_vec3 a, t_vec3 b, float t);
static inline t_vec3 vec3_schlick(t_vec3 f0, float u);
static inline t_vec3 vec3_bias(t_vec3 vec, t_vec3 n);
static inline uint32_t vec3_to_uint32(t_vec3 vec);
static inline float vec3_dist_sq(t_vec3 a, t_vec3 b);
static inline bool vec3_is_nan_inf(t_vec3 vec);
static inline float vec3_dist(t_vec3 a, t_vec3 b);
static inline t_vec3 vec3_fabsf(t_vec3 vec);
static inline t_vec3 vec3_powf(t_vec3 vec, float power);

// vec4
// -----------------------------------------------------------------
static inline t_vec4 vec4(float x, float y, float z, float w);
static inline t_vec4 vec4_3(t_vec3 vec, float w);
static inline t_vec4 vec4_n(float n);
static inline t_vec4 vec4_add(t_vec4 a, t_vec4 b);
static inline t_vec4 vec4_sub(t_vec4 a, t_vec4 b);
static inline t_vec4 vec4_scale(t_vec4 vec, float s);
static inline float vec4_dot(t_vec4 a, t_vec4 b);

// v4sf, v4si
// -----------------------------------------------------------------
static inline t_v4sf v4sf(float x, float y, float z, float w);
static inline t_v4sf v4sf_n(float n);
static inline t_v4si v4si(int32_t x, int32_t y, int32_t z, int32_t w);
static inline t_v4si v4si_n(int32_t n);

// Random
// -----------------------------------------------------------------
static inline uint32_t pcg(uint32_t* state);
static inline float randomf(uint32_t* seed);
static inline float randomf01(uint32_t* seed);
static inline float randomfn11(uint32_t* seed);
static inline uint32_t hash_lowerbias32(uint32_t seed);
static inline uint32_t fast_range(uint32_t n, uint32_t range);

// Fast approximations
// -----------------------------------------------------------------
static inline float fast_atan2f(float y, float x);
static inline float fast_acosf(float x);
static inline float fast_srgb(float x);

// Colors
// -----------------------------------------------------------------
static inline t_vec3 lerp_color(uint32_t color1, uint32_t color2, float t);

// Coordinate systems
// -----------------------------------------------------------------
static inline t_vec3 spherical_to_cartesian(float u, float sin_theta, float cos_theta);
static inline t_vec3 spherical_to_world(t_vec3 n, float u, float sin_theta, float cos_theta);
static inline t_vec3 tangent_to_world(t_vec3 vec, t_vec3 n);
static inline void onb(t_vec3 n, t_vec3* b1, t_vec3* b2);
static inline t_vec3 mul_tbn(t_vec3 vec, t_vec3 n, t_vec3 t, t_vec3 b);

// UVs
static inline t_vec3 map_spherical(float u, float v);
static inline t_vec2 spherical_uv(t_vec3 dir);

// Quaternions
// -----------------------------------------------------------------
static inline t_quat quat_from_euler(t_vec3 euler);
static inline t_quat quat_from_euler_angle(t_vec3 axis, float angle);
static inline t_quat quat_normalize(t_quat q);
static inline t_mat4 quat_to_mat4(t_quat q);
static inline t_quat quat_mul(t_quat a, t_quat b);
static inline t_quat quat_from_dir(t_vec3 dir);
#pragma endregion

static inline t_v4sf v4sf(float x, float y, float z, float w) {
	return (t_v4sf){ x, y, z, w };
}

static inline t_v4sf v4sf_n(float n) {
	return (t_v4sf){ n, n, n, n };
}

static inline t_v4si v4si(int32_t x, int32_t y, int32_t z, int32_t w) {
	return (t_v4si){ x, y, z, w };
}

static inline t_v4si v4si_n(int32_t n) {
	return (t_v4si){ n, n, n, n };
}

static inline t_vec2i vec2i(int32_t x, int32_t y) {
	return (t_vec2i){ x, y };
}

static inline t_vec2i vec2i_n(int32_t n) {
	return (t_vec2i){ n, n };
}

static inline t_vec2i vec2i_add(t_vec2i a, t_vec2i b) {
	return (t_vec2i){ a.x + b.x, a.y + b.y };
}

static inline t_vec2i vec2i_sub(t_vec2i a, t_vec2i b) {
	return (t_vec2i){ a.x - b.x, a.y - b.y };
}

static inline t_vec2i vec2i_scale(t_vec2i vec, float s) {
	return (t_vec2i){ (int)((float)vec.x * s), (int)((float)vec.y * s) };
}

static inline t_vec2i vec2i_f(float x, float y) {
	return (t_vec2i){ (int32_t)x, (int32_t)y };
}

static inline t_vec2i vec2i_nf(float n) {
	return (t_vec2i){ (int32_t)n, (int32_t)n };
}

static inline t_vec2 vec2(float x, float y) {
	return (t_vec2){ { x, y } };
}

static inline t_vec2 vec2_n(float n) {
	return (t_vec2){ { n, n } };
}

static inline t_vec2 vec2_add(t_vec2 a, t_vec2 b) {
	return (t_vec2){ { a.x + b.x, a.y + b.y } };
}

static inline t_vec2 vec2_sub(t_vec2 a, t_vec2 b) {
	return (t_vec2){ { a.x - b.x, a.y - b.y } };
}

static inline t_vec2 vec2_scale(t_vec2 vec, float s) {
	return (t_vec2){ { vec.x * s, vec.y * s } };
}

static inline t_vec3 vec3(float x, float y, float z) {
	return (t_vec3){ { x, y, z, 0.0f } };
}

static inline t_vec3 vec3_n(float n) {
	return (t_vec3){ { n, n, n, 0.0f } };
}

static inline t_vec3 vec3_add(t_vec3 a, t_vec3 b) {
	return (t_vec3){ .v = a.v + b.v };
}

static inline t_vec3 vec3_sub(t_vec3 a, t_vec3 b) {
	return (t_vec3){ .v = a.v - b.v };
}

static inline t_vec3 vec3_scale(t_vec3 vec, float s) {
	t_vec3 factor = { .v = v4sf_n(s) };
	return vec3_mul(vec, factor);
}

static inline t_vec3 vec3_cross(t_vec3 a, t_vec3 b) {
	t_v4sf yzx = _mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(3, 0, 2, 1)) * _mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(3, 1, 0, 2));
	t_v4sf zxy = _mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(3, 1, 0, 2)) * _mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(3, 0, 2, 1));
	return (t_vec3){ .v = yzx - zxy };
}

static inline float vec3_dot(t_vec3 a, t_vec3 b) {
	t_v4sf res = a.v * b.v;
	return res[0] + res[1] + res[2];
}

static inline t_vec3 vec3_div(t_vec3 vec, float den) {
	if (fabsf(den) < LEN_EPSILON)
		return (t_vec3){ 0 };

	return vec3_scale(vec, 1.0f / den);
}

static inline t_vec3 vec3_normalize(t_vec3 vec) {
	float len_sq = vec3_dot(vec, vec);
	if (len_sq < LEN_SQ_EPSILON)
		return (t_vec3){ 0 };

	float len_recip = 1.0f / sqrtf(len_sq);
	return vec3_scale(vec, len_recip);
}

static inline float vec3_length(t_vec3 vec) {
	return sqrtf(vec3_dot(vec, vec));
}

static inline t_vec3 vec3_mul(t_vec3 a, t_vec3 b) {
	return (t_vec3){ .v = a.v * b.v };
}

static inline t_vec3 vec3_reflect(t_vec3 vec, t_vec3 n) {
	return vec3_sub(vec, vec3_scale(n, 2 * vec3_dot(vec, n)));
}

static inline t_vec3 vec3_unit_random(uint32_t* seed) {
	while (true) {
		t_vec3 res = vec3(randomfn11(seed), randomfn11(seed), randomfn11(seed));
		float len_sq = vec3_dot(res, res);
		if (len_sq <= 1.0f && len_sq >= LEN_SQ_EPSILON)
			return vec3_div(res, sqrtf(len_sq));
	}
}

static inline t_vec3 vec3_min(t_vec3 vec, float min) {
	t_v4sf limit = v4sf(min, min, min, M_INFf);
	return (t_vec3){ .v = _mm_min_ps(vec.v, limit) };
}

static inline t_vec3 vec3_max(t_vec3 vec, float max) {
	t_v4sf limit = v4sf(max, max, max, -M_INFf);
	return (t_vec3){ .v = _mm_max_ps(vec.v, limit) };
}

static inline t_vec3 vec3_sqrt(t_vec3 vec) {
	return (t_vec3){ .v = _mm_sqrt_ps(vec.v) };
}

static inline t_vec3 vec3_clamp(t_vec3 vec, float min, float max) {
	t_vec3 res;
	res.v = _mm_max_ps(vec.v, v4sf_n(min));
	res.v = _mm_min_ps(res.v, v4sf_n(max));
	return res;
}

static inline t_vec3 vec3_clamp01(t_vec3 vec) {
	return vec3_clamp(vec, 0.0f, 1.0f);
}

static inline t_vec3 vec3_clamp_mag(t_vec3 vec, float max) {
	float len_sq = vec3_dot(vec, vec);
	if (len_sq > max * max) {
		float scale = max / sqrtf(len_sq);
		return vec3_scale(vec, scale);
	}
	return vec;
}

static inline t_vec3 vec3_add_n(t_vec3 vec, float n) {
	return (t_vec3){ .v = vec.v + v4sf_n(n) };
}

static inline t_vec3 vec3_negate(t_vec3 vec) {
	return vec3_scale(vec, -1.0f);
}

static inline t_vec3 vec3_lerp(t_vec3 a, t_vec3 b, float t) {
	return (t_vec3){ .v = a.v + (b.v - a.v) * v4sf_n(t) };
}

static inline t_vec3 vec3_schlick(t_vec3 f0, float u) {
	float t = 1.0f - u;
	float t2 = t * t;
	float f = t2 * t2 * t;
	return vec3_add(vec3_scale(f0, (1.0f - f)), vec3_n(f));
}

static inline t_vec3 vec3_bias(t_vec3 vec, t_vec3 n) {
	t_vec3 abs = vec3_fabsf(vec);
	float max_coord = fmaxf(fmaxf(abs.x, abs.y), abs.z);
	float bias = max_coord * G_EPSILON;
	bias = fmaxf(B_EPSILON, bias);
	return vec3_add(vec, vec3_scale(n, bias));
}

static inline uint32_t vec3_to_uint32(t_vec3 vec) {
	__m128i res;
	res = _mm_cvtps_epi32(vec.v * v4sf_n(255.0f));
	res = _mm_packs_epi32(res, res);
	res = _mm_packus_epi16(res, res);
	return (uint32_t)_mm_cvtsi128_si32(res) | 0xFF000000;
}

static inline float vec3_dist(t_vec3 a, t_vec3 b) {
	return sqrtf(vec3_dist_sq(a, b));
}

static inline float vec3_dist_sq(t_vec3 a, t_vec3 b) {
	t_vec3 diff = vec3_sub(b, a);
	return vec3_dot(diff, diff);
}

static inline bool vec3_is_nan_inf(t_vec3 vec) {
	static const uint32_t e_bits = 0x7F800000u;

	t_v4ui_ui v_s;
	v_s.v = (t_v4ui)vec.v;
	v_s.v &= e_bits;
	return (v_s.s[0] == e_bits) | (v_s.s[1] == e_bits) | (v_s.s[2] == e_bits) | (v_s.s[3] == e_bits);
}

static inline t_vec3 vec3_fabsf(t_vec3 vec) {
	return vec3(fabsf(vec.x), fabsf(vec.y), fabsf(vec.z));
}

static inline t_vec3 vec3_powf(t_vec3 vec, float power) {
	return vec3(powf(vec.x, power), powf(vec.y, power), powf(vec.z, power));
}

static inline t_vec4 vec4(float x, float y, float z, float w) {
	return (t_vec4){ { x, y, z, w } };
}

static inline t_vec4 vec4_n(float n) {
	return (t_vec4){ { n, n, n, n } };
}

static inline t_vec4 vec4_add(t_vec4 a, t_vec4 b) {
	return (t_vec4){ .v = a.v + b.v };
}

static inline t_vec4 vec4_sub(t_vec4 a, t_vec4 b) {
	return (t_vec4){ .v = a.v - b.v };
}

static inline t_vec4 vec4_scale(t_vec4 vec, float s) {
	return (t_vec4){ .v = vec.v * v4sf_n(s) };
}

static inline t_vec4 vec4_3(t_vec3 vec, float w) {
	t_vec4 res = { vec.v };
	res.w = w;
	return res;
}

static inline float vec4_dot(t_vec4 a, t_vec4 b) {
	t_v4sf res = a.v * b.v;
	return res[0] + res[1] + res[2] + res[3];
}

static inline t_mat4 mat4_identity(void) {
	static const t_v4sf r0 = { 1.0f, 0.0f, 0.0f, 0.0f };
	static const t_v4sf r1 = { 0.0f, 1.0f, 0.0f, 0.0f };
	static const t_v4sf r2 = { 0.0f, 0.0f, 1.0f, 0.0f };
	static const t_v4sf r3 = { 0.0f, 0.0f, 0.0f, 1.0f };

	return (t_mat4){ //
		.rows[0] = r0,
		.rows[1] = r1,
		.rows[2] = r2,
		.rows[3] = r3
	};
}

static inline t_mat4 mat4_transpose(const t_mat4* src) {
	__m128 c0 = (__m128)src->rows[0];
	__m128 c1 = (__m128)src->rows[1];
	__m128 c2 = (__m128)src->rows[2];
	__m128 c3 = (__m128)src->rows[3];
	_MM_TRANSPOSE4_PS(c0, c1, c2, c3);

	return (t_mat4){ //
		.rows[0] = (t_v4sf)c0,
		.rows[1] = (t_v4sf)c1,
		.rows[2] = (t_v4sf)c2,
		.rows[3] = (t_v4sf)c3
	};
}

static inline t_mat4 mat4_mul(const t_mat4* a, const t_mat4* b) {
	t_mat4 res;
	int i = 0;
	while (i < 4) {
		t_v4sf a_row = a->rows[i];
		res.rows[i] = v4sf_n(a_row[0]) * b->rows[0];
		res.rows[i] += v4sf_n(a_row[1]) * b->rows[1];
		res.rows[i] += v4sf_n(a_row[2]) * b->rows[2];
		res.rows[i] += v4sf_n(a_row[3]) * b->rows[3];
		++i;
	}
	return res;
}

static inline t_vec4 mat4_mul_vec4(const t_mat4* src, t_vec4 vec) {
	t_mat4 t = mat4_transpose(src);
	// clang-format off
	t_vec4 res;
	res.v = t.rows[0] * v4sf_n(vec.x) +
			t.rows[1] * v4sf_n(vec.y) +
			t.rows[2] * v4sf_n(vec.z) +
			t.rows[3] * v4sf_n(vec.w);
	// clang-format on
	return res;
}

static inline t_vec3 mat4_mul_vec3(const t_mat4* src, t_vec3 vec) {
	t_vec4 temp = { .v = vec.v };
	return (t_vec3){ .v = mat4_mul_vec4(src, temp).v };
}

static inline bool mat4_inverse(const t_mat4* src, t_mat4* out) {
	float det = mat4_det(src);
	if (fabsf(det) < M_EPSILON)
		return false;

	float inv_det = 1.0f / det;
	int row = 0;
	while (row < 4) {
		int col = 0;
		while (col < 4) {
			float cofactor = mat4_cofactor(src, row, col);
			out->m[col][row] = cofactor * inv_det;
			++col;
		}
		++row;
	}
	return true;
}

static inline t_mat3 mat4_sub(const t_mat4* src, int skip_r, int skip_c) {
	t_mat3 res;
	int row = 0;
	int r = 0;
	while (row < 4) {
		if (row == skip_r && ++row)
			continue;
		int c = 0;
		int col = 0;
		while (col < 4) {
			if (col == skip_c && ++col)
				continue;
			res.m[r][c++] = src->m[row][col++];
		}
		++r;
		++row;
	}
	return res;
}

static inline float mat4_cofactor(const t_mat4* src, int row, int col) {
	t_mat3 sub = mat4_sub(src, row, col);
	float det = mat3_det(&sub);
	if ((row + col) & 1)
		return -det;
	else
		return det;
}

static inline float mat4_det(const t_mat4* src) {
	float res = 0.0f;
	int i = 0;
	while (i < 4) {
		res += src->m[0][i] * mat4_cofactor(src, 0, i);
		++i;
	}
	return res;
}

static inline float mat3_det(const t_mat3* src) {
	float res;
	res = src->m[0][0] * (src->m[1][1] * src->m[2][2] - src->m[2][1] * src->m[1][2]);
	res -= src->m[0][1] * (src->m[1][0] * src->m[2][2] - src->m[2][0] * src->m[1][2]);
	res += src->m[0][2] * (src->m[1][0] * src->m[2][1] - src->m[2][0] * src->m[1][1]);
	return res;
}

static inline t_mat4 mat4_translate(t_vec3 t) {
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ 1.0f, 0.0f, 0.0f, t.x },
		.rows[1] = (t_v4sf){ 0.0f, 1.0f, 0.0f, t.y },
		.rows[2] = (t_v4sf){ 0.0f, 0.0f, 1.0f, t.z },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}

static inline t_mat4 mat4_scale(t_vec3 s) {
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ s.x, 0.0f, 0.0f, 0.0f },
		.rows[1] = (t_v4sf){ 0.0f, s.y, 0.0f, 0.0f },
		.rows[2] = (t_v4sf){ 0.0f, 0.0f, s.z, 0.0f },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}

static inline t_mat4 mat4_rot_x(float rad) {
	float s = sinf(rad);
	float c = cosf(rad);
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ 1.0f, 0.0f, 0.0f, 0.0f },
		.rows[1] = (t_v4sf){ 0.0f, c, -s, 0.0f },
		.rows[2] = (t_v4sf){ 0.0f, s, c, 0.0f },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}

static inline t_mat4 mat4_rot_y(float rad) {
	float s = sinf(rad);
	float c = cosf(rad);
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ c, 0.0f, s, 0.0f },
		.rows[1] = (t_v4sf){ 0.0f, 1.0f, 0.0f, 0.0f },
		.rows[2] = (t_v4sf){ -s, 0.0f, c, 0.0f },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}

static inline t_mat4 mat4_rot_z(float rad) {
	float s = sinf(rad);
	float c = cosf(rad);
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ c, -s, 0.0f, 0.0f },
		.rows[1] = (t_v4sf){ s, c, 0.0f, 0.0f },
		.rows[2] = (t_v4sf){ 0.0f, 0.0f, 1.0f, 0.0f },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}

static inline t_mat4 mat4_rot(t_vec3 rot) {
	t_vec2 x = { .sin = sinf(rot.x), .cos = cosf(rot.x) };
	t_vec2 y = { .sin = sinf(rot.y), .cos = cosf(rot.y) };
	t_vec2 z = { .sin = sinf(rot.z), .cos = cosf(rot.z) };
	return (t_mat4){ //
		.rows[0] = v4sf(y.cos * z.cos, z.cos * x.sin * y.sin - x.cos * z.sin, x.cos * z.cos * y.sin + x.sin * z.sin, 0.0f),
		.rows[1] = v4sf(y.cos * z.sin, x.cos * z.cos + x.sin * y.sin * z.sin, -z.cos * x.sin + x.cos * y.sin * z.sin, 0.0f),
		.rows[2] = v4sf(-y.sin, y.cos * x.sin, x.cos * y.cos, 0.0f),
		.rows[3] = v4sf(0.0f, 0.0f, 0.0f, 1.0f)
	};
}

static inline t_mat4 mat4_shear(t_vec3 x, t_vec3 y, t_vec3 z) {
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ 1.0f, x.y, x.z, 0.0f },
		.rows[1] = (t_v4sf){ y.x, 1.0f, y.z, 0.0f },
		.rows[2] = (t_v4sf){ z.x, z.y, 1.0f, 0.0f },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}

static inline t_quat quat_from_euler(t_vec3 euler) {
	euler = vec3_scale(euler, 0.5f);
	t_vec2 x = { .sin = sinf(euler.x), .cos = cosf(euler.x) };
	t_vec2 y = { .sin = sinf(euler.y), .cos = cosf(euler.y) };
	t_vec2 z = { .sin = sinf(euler.z), .cos = cosf(euler.z) };
	return (t_quat){ //
		.w = x.cos * y.cos * z.cos + x.sin * y.sin * z.sin,
		.x = x.sin * y.cos * z.cos - x.cos * y.sin * z.sin,
		.y = x.cos * y.sin * z.cos + x.sin * y.cos * z.sin,
		.z = x.cos * y.cos * z.sin - x.sin * y.sin * z.cos
	};
}

static inline t_quat quat_from_euler_angle(t_vec3 axis, float angle) {
	float half_angle = angle * 0.5f;
	t_vec2 theta = { .sin = sinf(half_angle), .cos = cosf(half_angle) };
	axis = vec3_normalize(axis);
	t_vec3 imag = vec3_scale(axis, theta.sin);
	return (t_quat){ //
		.x = imag.x,
		.y = imag.y,
		.z = imag.z,
		.w = theta.cos
	};
}

static inline t_quat quat_normalize(t_quat q) {
	float len_sq = vec4_dot(q, q);
	if (len_sq > LEN_SQ_EPSILON)
		q = vec4_scale(q, 1.0f / sqrtf(len_sq));
	return q;
}

static inline t_mat4 quat_to_mat4(t_quat q) {
	t_mat4 m = mat4_identity();
	t_vec3 x = vec3(q.x * q.x, q.x * q.y, q.x * q.z);
	t_vec3 yz = vec3(q.y * q.z, q.y * q.y, q.z * q.z);
	t_vec3 w = vec3(q.w * q.x, q.w * q.y, q.w * q.z);
	m.m[0][0] = 1.0f - 2.0f * (yz.y + yz.z);
	m.m[0][1] = 2.0f * (x.y - w.z);
	m.m[0][2] = 2.0f * (x.z + w.y);
	m.m[1][0] = 2.0f * (x.y + w.z);
	m.m[1][1] = 1.0f - 2.0f * (x.x + yz.z);
	m.m[1][2] = 2.0f * (yz.x - w.x);
	m.m[2][0] = 2.0f * (x.z - w.y);
	m.m[2][1] = 2.0f * (yz.x + w.x);
	m.m[2][2] = 1.0f - 2.0f * (x.x + yz.y);
	return m;
}

static inline t_quat quat_mul(t_quat a, t_quat b) {
	return (t_quat){ //
		.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
		.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
		.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
		.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w
	};
}

static inline t_quat quat_from_dir(t_vec3 dir) {
	const t_vec3 up = { { 0.0f, 1.0f, 0.0f, 0.0f } };

	dir = vec3_normalize(dir);
	float dot = vec3_dot(up, dir);
	if (dot < -0.999f)
		return quat_from_euler(vec3(M_PIf, 0.0f, 0.0f));

	t_vec3 cross = vec3_cross(up, dir);
	t_quat q = { //
		.x = cross.x,
		.y = cross.y,
		.z = cross.z,
		.w = 1.0f + dot
	};
	return quat_normalize(q);
}

static inline float ft_lerp(float a, float b, float t) {
	return (1.0f - t) * a + t * b;
}

static inline int ft_imax(int a, int b) {
	return a > b ? a : b;
}

static inline int ft_imin(int a, int b) {
	return a > b ? b : a;
}

static inline size_t ft_pow(size_t n, size_t e) {
	size_t result = 1;
	size_t i = 0;
	while (i < e) {
		result *= n;
		++i;
	}
	return result;
}

static inline bool is_pot(size_t n) {
	return n != 0 && ((n & (n - 1)) == 0);
}

static inline float ft_normalize(float value, float min, float max) {
	return clampf01((value - min) / (max - min));
}

static inline float clampf01(float value) {
	return clampf(value, 0.0f, 1.0f);
}

static inline float clampf(float value, float min, float max) {
	value = fminf(value, max);
	value = fmaxf(value, min);
	return value;
}

static inline float clampfn11(float value) {
	return clampf(value, -1.0f, 1.0f);
}

static inline uint32_t ft_uint_min(uint32_t a, uint32_t b) {
	return a < b ? a : b;
}

static inline float ft_lerp_fast(float a, float b, float t) {
	return a + (b - a) * t;
}

static inline float f_schlick(float u, float f0, float f90) {
	float t = 1.0f - u;
	float t2 = t * t;
	return f0 + (f90 - f0) * t2 * t2 * t;
}

static inline float degrees_to_rad(float degrees) {
	return degrees * M_PIf / 180.0f;
}

static inline float rad_to_degrees(float rad) {
	return rad * 180.0f * M_1_PIf;
}

static inline bool is_nan_inf(float x) {
	t_f_ui n = { .f = x };
	return (n.i & 0x7F800000) == 0x7F800000;
}

static inline float fast_atan2f(float y, float x) {
	float abs_x = fabsf(x) + 1e-10f;
	float abs_y = fabsf(y);
	float a = fminf(abs_x, abs_y) / fmaxf(abs_x, abs_y);
	float s = a * a;
	float r = ((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a;
	if (abs_y > abs_x)
		r = M_PI_2f - r;
	if (x < 0.0f)
		r = M_PIf - r;
	if (y < 0.0f)
		r = -r;
	return r;
}

static inline float fast_acosf(float x) {
	float negate = (float)(x < 0.0f);
	x = fabsf(x);
	float ret = -0.0187293f;
	ret = ret * x + 0.0742610f;
	ret = ret * x - 0.2121144f;
	ret = ret * x + 1.5707288f;
	ret = ret * sqrtf(1.0f - x);
	ret = ret - 2.0f * negate * ret;
	return negate * M_PIf + ret;
}

static inline float fast_srgb(float x) {
	if (x <= 0.0f)
		return 0.0f;

	float s = sqrtf(x);
	float q = sqrtf(s);
	float sq = s * q;
	return 0.04400f * q + 1.17400f * s - 0.28500f * sq + 0.06700f * x;
}

// Stateless mixer for seed hashing by Marc B. Reynolds
static inline uint32_t hash_lowerbias32(uint32_t seed) {
	seed ^= seed >> 16;
	seed *= 0xa812d533;
	seed ^= seed >> 15;
	seed *= 2994267309u;
	seed ^= seed >> 17;
	return seed;
}

// Permutated Congruential Generator for sequences
static inline uint32_t pcg(uint32_t* state) {
	uint32_t old_state = *state;
	*state = old_state * 747796405u + 2891336453u;
	uint32_t word = ((old_state >> ((old_state >> 28u) + 4u)) ^ old_state) * 277803737u;
	return (word >> 22u) ^ word;
}

static inline float randomf(uint32_t* seed) {
	return (float)pcg(seed);
}

static inline float randomf01(uint32_t* seed) {
	return randomf(seed) / (float)UINT32_MAX;
}

static inline float randomfn11(uint32_t* seed) {
	return (randomf01(seed) - 0.5f) * 2.0f;
}

// Lemire's Method
static inline uint32_t fast_range(uint32_t n, uint32_t range) {
	return (uint32_t)(((uint64_t)n * (uint64_t)range) >> 32);
}

static inline t_vec3 spherical_to_cartesian(float u, float sin_theta, float cos_theta) {
	float angle = M_TAUf * u;
	t_vec2 phi = { .sin = sinf(angle), .cos = cosf(angle) };
	return (t_vec3){ //
		.x = sin_theta * phi.cos,
		.y = sin_theta * phi.sin,
		.z = cos_theta
	};
}

static inline t_vec3 spherical_to_world(t_vec3 n, float u, float sin_theta, float cos_theta) {
	t_vec3 local = spherical_to_cartesian(u, sin_theta, cos_theta);
	return tangent_to_world(local, n);
}

static inline t_vec3 tangent_to_world(t_vec3 vec, t_vec3 n) {
	t_vec3 t, b;
	onb(n, &t, &b);
	return mul_tbn(vec, n, t, b);
}

// Pixar Orthonormal Basis, Duff et al.
static inline void onb(t_vec3 n, t_vec3* b1, t_vec3* b2) {
	float sign = copysignf(1.0f, n.z);
	float a = -1.0f / (sign + n.z);
	float b = n.x * n.y * a;
	*b1 = vec3(1.0f + sign * n.x * n.x * a, sign * b, -sign * n.x);
	*b2 = vec3(b, sign + n.y * n.y * a, -n.y);
}

static inline t_vec3 mul_tbn(t_vec3 vec, t_vec3 n, t_vec3 t, t_vec3 b) {
	t_vec3 res = vec3_add(vec3_scale(t, vec.x), vec3_scale(b, vec.y));
	res = vec3_add(res, vec3_scale(n, vec.z));
	return res;
}

static inline t_vec3 map_spherical(float u, float v) {
	float cos_theta = 1.0f - 2.0f * v;
	float sin_theta = sqrtf(fmaxf(0.0f, 1.0f - cos_theta * cos_theta));
	float angle = M_TAUf * u;
	t_vec2 phi = { .sin = sinf(angle), .cos = cosf(angle) };
	return vec3(sin_theta * phi.cos, sin_theta * phi.sin, cos_theta);
}

static inline t_vec2 spherical_uv(t_vec3 dir) {
	return (t_vec2){ //
		.u = (fast_atan2f(dir.z, dir.x) + (float)M_PIf) * M_1_2PIf,
		.v = fast_acosf(clampfn11(dir.y)) * M_1_PIf
	};
}

static inline t_vec3 lerp_color(uint32_t c1, uint32_t c2, float t) {
	t_v4sf a = v4sf((c1 >> 24) & 0xFF, (c1 >> 16) & 0xFF, (c1 >> 8) & 0xFF, 255.0f);
	t_v4sf b = v4sf((c2 >> 24) & 0xFF, (c2 >> 16) & 0xFF, (c2 >> 8) & 0xFF, 255.0f);
	return (t_vec3){ .v = ((1.0f - t) * a + t * b) * M_1_255f };
}

#endif
