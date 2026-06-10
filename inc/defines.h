#ifndef DEFINES_H
#define DEFINES_H

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

#include "MLX42.h"
#include "OpenImageDenoise/oidn.h"
#include "lib_math.h"
#include "libft_defs.h"
#include "libft_vector.h"

#define WIDTH 1920
#define HEIGHT 1080
#define UI_WIDTH 350
#define UI_BOTTOM 28
#define THREADS_DFL 4
#define TILE_SIZE 32
#define RENDER_SAMPLES 32
#define PREVIEW_BOUNCES 2
#define SOLID_BOUNCES 1
#define RENDER_BOUNCES 8
#define DEPTH_ENABLE_RR 3

#define MAX_NAME_LEN 64
#define MAX_TEXTURES 64

#define SENS_TRANSLATE 0.0015f
#define SENS_ROTATE 0.003f
#define SENS_SCALE 0.0019f
#define SENS_LOOK 0.0023f
#define SENS_ORBIT 0.003f
#define SENS_ZOOM 0.0035f
#define SENS_PAN 0.0005f
#define SENS_MOVE 1.0f

#define SENSOR_HEIGHT_MM 24.0f
#define SENSOR_HALF_HEIGHT_MM 12.0f

#define MAX_RADIANCE 10.0f
#define CLAMP_INDIRECT 10.0f
#define WORLD_LIMIT 9999.9f

#define KEY_FORWARD MLX_KEY_W
#define KEY_LEFT MLX_KEY_A
#define KEY_BACK MLX_KEY_S
#define KEY_RIGHT MLX_KEY_D
#define KEY_UP MLX_KEY_SPACE
#define KEY_DOWN MLX_KEY_LEFT_SHIFT
#define KEY_RESET MLX_KEY_R

#define OBJ_HIDDEN_SCENE 1
#define OBJ_HIDDEN_CAM 2
#define OBJ_NO_CAST_SHADOW 4
#define MAT_NO_REC_SHADOW 8
#define MAT_DOUBLE_SIDED 16

#define UINT_PRIME 15485863u
#define FP_PRIME 1619u

enum e_err_code {
	ERR_ARGINVL,
	ERR_MLXINIT,
	ERR_IMGINIT,
	ERR_VECINIT,
	ERR_VECADD,
	ERR_RESIZE,
	ERR_SIZE_MAX,
	ERR_WRITE,
	ERR_OPEN,
	ERR_READ,
	ERR_GNL,
	ERR_TEX,
	ERR_EINVAL,
	ERR_RENINIT,
	ERR_OBJADD,
	ERR_POINTLADD,
	ERR_BVH,
	ERR_MATADD,
	ERR_TEXNPOT,
	ERR_DIR,
	ERR_MALLOC,
};

enum e_obj_type {
	OBJ_PLANE,
	OBJ_SPHERE,
	OBJ_CYLINDER,
	OBJ_CONE,
	OBJ_QUAD,
	OBJ_MESH,
};

enum e_light_type {
	LIGHT_POINT,
	LIGHT_AMBIENT,
	LIGHT_AREA,
	LIGHT_DIRECTIONAL,
};

enum e_cam_state {
	CAM_DEFAULT,
	CAM_TURN,
	CAM_LOOK,
	CAM_ORBIT,
	CAM_ZOOM,
	CAM_PAN,
};

enum e_base_color {
	BASE_COLOR,
	BASE_PATTERN,
	BASE_TEXTURE,
};

enum e_pattern {
	PAT_NONE,
	PAT_CHECKERBOARD,
	PAT_GRADIENT,
	PAT_STRIPES,
	PAT_SPIRAL,
	PAT_GRID,
	PAT_BRICK,
	PAT_MARBLE,
	PAT_WOOD,
	PAT_TURBULENCE,
};

enum e_entity {
	ENT_CAMERA,
	ENT_LIGHT,
	ENT_OBJECT,
};

enum e_render_mode {
	RENDERED,
	PREVIEW,
	SOLID,
};

enum e_bn_channel {
	BN_PX_U,
	BN_PX_V,
	BN_DD_U,
	BN_DD_V,
	BN_SC_U,
	BN_SC_V,
	BN_SR_U,
	BN_LI,
	BN_PP_R,
	BN_PP_G,
	BN_PP_B,
	BN_PP_PAD,
	BN_CO_U,
};

enum e_edit_mode {
	EDIT_DEFAULT,
	EDIT_TRANSLATE,
	EDIT_ROTATE,
	EDIT_SCALE,
};

enum e_axis {
	AXIS_DEFAULT,
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_XY,
	AXIS_XZ,
	AXIS_YZ,
};

static const t_vec3 g_zero = { { 0.0f, 0.0f, 0.0f, 0.0f } };
static const t_vec3 g_one = { { 1.0f, 1.0f, 1.0f, 0.0f } };
static const t_vec3 g_right = { { 1.0f, 0.0f, 0.0f, 0.0f } };
static const t_vec3 g_up = { { 0.0f, 1.0f, 0.0f, 0.0f } };
static const t_vec3 g_forward = { { 0.0f, 0.0f, 1.0f, 0.0f } };
static const t_vec3 g_world_limit = { { WORLD_LIMIT, WORLD_LIMIT, WORLD_LIMIT, 1.0f } };
static const t_vec3 g_world_limit_neg = { { -WORLD_LIMIT, -WORLD_LIMIT, -WORLD_LIMIT, 1.0f } };

typedef enum e_obj_type t_obj_type;
typedef enum e_light_type t_light_type;
typedef enum e_cam_state t_cam_state;
typedef enum e_base_color t_base_color;
typedef enum e_pattern t_pattern;
typedef enum e_entity t_entity;
typedef enum e_err_code t_err_code;
typedef enum e_render_mode t_render_mode;
typedef enum e_bn_channel t_bn_channel;
typedef enum e_edit_mode t_edit_mode;
typedef enum e_axis t_axis;

typedef struct s_context t_context;
typedef struct s_bvh_node t_bvh_node;
typedef struct s_aabb t_aabb;
typedef struct s_object t_object;
typedef struct s_hit t_hit;
typedef struct s_plane t_plane;
typedef struct s_sphere t_sphere;
typedef struct s_cylinder t_cylinder;
typedef struct s_cone t_cone;
typedef struct s_quad t_quad;
typedef struct s_triangle t_triangle;
typedef struct s_mesh t_mesh;
typedef struct s_light t_light;
typedef struct s_scene t_scene;
typedef struct s_camera t_camera;
typedef struct s_texture t_texture;
typedef struct s_material t_material;
typedef struct s_renderer t_renderer;
typedef struct s_editor t_editor;
typedef struct s_viewport t_viewport;
typedef struct s_path t_path;
typedef struct s_pixel t_pixel;
typedef struct s_ray t_ray;
typedef struct s_transform t_transform;
typedef struct s_tex_entry t_tex_entry;
typedef struct s_geo t_geo;
typedef struct s_env t_env;
typedef struct s_assets t_assets;
typedef struct s_mouse t_mouse;
typedef struct s_bvh_element t_bvh_element;

typedef union u_shape t_shape;
typedef union u_ui t_ui;

typedef mlx_image_t t_image;

union u_ui {
	struct {
		float* lastf;
		float currentf;
	};
	struct {
		int32_t* last;
		int32_t current;
	};
};

struct __attribute__((aligned(16))) s_transform {
	t_mat4 world_to_object;
	t_mat4 object_to_world;
	t_vec3 pos;
	t_quat rot;
	t_vec3 rot_euler;
	t_vec3 scale;
};

struct __attribute__((aligned(16))) s_ray {
	t_vec3 origin;
	t_vec3 dir;
	t_vec3 dir_recip;
	t_v4ui signs;
};
struct __attribute__((aligned(16))) s_hit {
	float t;
	bool is_primary;
	bool front_face;
	t_object* obj;
	t_vec3 point;
	t_vec3 normal;
	t_vec3 tangent;
	t_vec3 bitangent;
	t_vec2 uv;
};

struct s_bvh_element {
	uint32_t stack[64];
	t_bvh_node* node;
	t_hit temp;
	bool res;
	int32_t i;
};

struct __attribute__((aligned(16))) s_texture {
	mlx_texture_t* tex;
	float* pixels;
	uint32_t width;
	uint32_t height;
};

struct __attribute__((aligned(16))) s_material {
	t_texture texture;
	t_texture normal_map;
	t_vec3 albedo;
	t_vec3 albedo2;
	t_vec3 emission;
	t_vec3 f0_dielectric;
	float metallic;
	float roughness;
	float ior;
	float transmission;
	float pattern_scale;
	float bump_strength;
	uint32_t flags;
	t_base_color base_color;
	t_pattern pattern;
	bool is_emissive;
};

struct __attribute__((aligned(16))) s_plane {
	t_vec3 point;
	t_vec3 normal;
	t_vec3 u;
	t_vec3 v;
	float d;
};

struct __attribute__((aligned(16))) s_sphere {
	float radius;
	float radius_sq;
};

struct __attribute__((aligned(16))) s_cylinder {
	t_vec3 axis;
	t_vec3 center;
	float radius;
	float height;
};

struct __attribute__((aligned(16))) s_cone {
	t_vec3 apex;
	t_vec3 axis;
	float angle;
	float height;
	float cos_sq;
	float tan_sq;
	float base_radius;
	float inv_height;
};

struct __attribute__((aligned(16))) s_quad {
	t_vec3 vec_alpha;
	t_vec3 vec_beta;
	t_vec3 q;
	t_vec3 u;
	t_vec3 v;
	t_vec3 normal;
	t_vec3 w;
	float d;
	float area;
};

struct __attribute__((aligned(16))) s_triangle {
	t_vec3 v0, v1, v2;
	t_vec3 n0, n1, n2;
	t_vec2 uv0, uv1, uv2;
	bool has_normals, has_uvs;
};

struct __attribute__((aligned(16))) s_mesh {
	char name[64];
	t_triangle* triangles;
	t_bvh_node* bvh_nodes;
	uint32_t bvh_root_idx;
	uint32_t nodes;
	uint32_t triangle_count;
};

union __attribute__((aligned(16))) u_shape {
	t_mesh mesh;
	t_plane plane;
	t_sphere sphere;
	t_cylinder cylinder;
	t_cone cone;
	t_quad quad;
};
struct __attribute__((aligned(16))) s_object {
	t_obj_type type;
	uint32_t flags;
	uint32_t material_id;
	t_material* mat;
	t_shape shape;
	t_transform transform;
	t_vec3 bounds_center;
	t_vec3 bounds;
};

struct __attribute__((aligned(16))) s_light {
	t_vec3 pos;
	t_vec3 color;
	t_vec3 emission;
	t_object* obj;
	uint32_t idx;
	float radius;
	float intensity;
	float max_radiance;
	float radius_sq;
	float angle;
	float cos_theta_max;
	t_light_type type;
};

struct __attribute__((aligned(16))) s_viewport {
	t_vec3 d_u;
	t_vec3 d_v;
	t_vec3 pixel_00_loc;
	float width;
	float height;
};

struct __attribute__((aligned(16))) s_camera {
	t_viewport viewport;
	t_light directional_light;
	t_transform transform;
	t_quat init_rot;
	t_vec3 up;
	t_vec3 right;
	t_vec3 forward;
	t_vec3 control_right;
	t_vec3 control_forward;
	t_vec3 defocus_disk_u;
	t_vec3 defocus_disk_v;
	t_vec3 init_pos;
	t_vec3 target;
	t_vec2 skydome_uv_offset;
	float init_focal_len_mm;
	float init_focus_dist;
	float aspect;
	float focal_len_mm;
	float f_stop;
	float pitch;
	float yaw;
	float focus_dist;
	float distance;
	float exposure;
	float shutter_speed;
	float iso;
	t_cam_state state;
};

struct __attribute__((aligned(16))) s_path {
	t_ray ray;
	t_hit hit;
	t_vec3 color;
	t_vec3 throughput;
	t_vec3 f0;
	t_vec3 n;
	t_vec3 l;
	t_vec3 v;
	t_vec3 h;
	t_vec2 uv;
	t_material* mat;
	uint32_t bounce;
	float ndotl;
	float ndotv;
	float ndoth;
	float vdoth;
	float ldoth;
	float alpha;
	float pdf;
	float p_spec;
	bool sample_spec;
};

struct s_tex_entry {
	char name[MAX_NAME_LEN];
	t_texture texture;
	bool loaded;
};

struct s_assets {
	t_vector materials;
	int tex_count;
	t_tex_entry textures[MAX_TEXTURES];
};

struct s_env {
	t_vector lights;
	t_light dir_light;
	t_light amb_light;
	t_texture skydome;
	t_vec3 amb_color_2;
	bool has_dir_light;
	bool show_background;
};

struct s_geo {
	t_vector objs;
	t_vector planes;
	t_bvh_node* bvh_nodes;
	uint32_t bvh_root_idx;
};

struct __attribute__((aligned(16))) s_scene {
	t_camera cam;
	t_geo geo;
	t_env env;
	t_assets assets;
};

struct __attribute__((aligned(64))) s_renderer {
	struct __attribute__((aligned(64))) {
		pthread_mutex_t mutex;
		pthread_cond_t cond;
		uint32_t tile_index;
		uint32_t threads_running;
		uint32_t tiles_total;
		bool active;
		bool resize_pending;
		bool frame_complete;
	};
	struct __attribute__((aligned(64))) {
		t_vec3* buffer;
		t_vec3* denoise_buffer;
		t_vec3* albedo_buffer;
		t_vec3* normal_buffer;
		t_camera cam;
		t_int2 tiles;
		uint32_t width;
		uint32_t height;
		uint32_t pixels;
		t_render_mode mode;
		uint32_t render_samples;
		uint32_t render_bounces;
		uint32_t frame;
		uint8_t ray_bounces;
		_Atomic bool render_cancel;
	};
	struct __attribute__((aligned(64))) {
		pthread_t* threads;
		uint32_t new_width;
		uint32_t new_height;
		int32_t threads_amount;
		int32_t threads_init;
		uint32_t render_time;
		uint32_t blit_time;
		bool init_mutex;
		bool init_cond;
	};
	struct __attribute__((aligned(64))) {
		OIDNDevice oidn_device;
		OIDNFilter oidn_filter;
		OIDNFilter oidn_filter_fast;
		OIDNBuffer oidn_buffer;
		OIDNBuffer oidn_albedo;
		OIDNBuffer oidn_normal;
	};
};

struct __attribute__((aligned(16))) s_editor {
	t_vec3 axis_primary;
	t_vec3 axis_secondary;
	t_transform orig_transform;
	float* selection_mask;
	t_object* selected_obj;
	t_edit_mode mode;
	t_axis constraint_axis;
	uint32_t constraints;
	bool request_ui_tab;
};

struct __attribute__((aligned(16))) s_pixel {
	t_vec3* color;
	uint32_t* seed;
	uint32_t x;
	uint32_t y;
	uint32_t frame;
	float u;
	float v;
	float scale;
};

struct __attribute__((aligned(16))) s_aabb {
	t_vec3 min;
	t_vec3 max;
};

struct __attribute__((aligned(16))) s_bvh_node {
	t_aabb aabb;
	union {
		t_object* obj;
		uint32_t tri_idx;
	};
	uint32_t left_idx;
	uint32_t right_idx;
	uint32_t tri_count;
	int axis;
};

struct s_mouse {
	t_vec2i pos;
	t_vec2i pos_prev;
	t_vec2i pos_orig;
	t_vec2i delta_accum;
};

struct __attribute__((aligned(64))) s_context {
	t_renderer renderer;
	t_scene scene;
	t_texture tex_bn;
	t_editor editor;
	t_mouse mouse;
	mlx_t* mlx;
	t_image* img;
	char* file;
	t_mesh* lib_mesh;
	uint32_t lib_mesh_capacity;
	uint32_t loaded_mesh_count;
	uint32_t resize_time;
	uint8_t bn_stride;
	int fd;
};

#endif
