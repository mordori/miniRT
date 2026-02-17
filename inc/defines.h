#ifndef DEFINES_H
# define DEFINES_H

# define _GNU_SOURCE

# include <pthread.h>
# include <stdatomic.h>

# include "MLX42.h"
# include "libft_defs.h"
# include "libft_vector.h"

# include "lib_math.h"

# define WIDTH					1920
# define HEIGHT					1080
# define THREADS_DFL			4
# define TILE_SIZE				32
# define RENDER_SAMPLES			1024
# define PREVIEW_BOUNCES		3
# define REFINE_BOUNCES			32
# define DEPTH_ENABLE_RR		3

# define MAX_NAME_LEN			64
# define MAX_TEXTURES			64

# define SENS_ORBIT				0.0025f
# define SENS_ZOOM				0.0018f
# define SENS_PAN				0.0006f
# define SENS_MOVE				5.0f

# define MAX_BRIGHTNESS			10.0f

# define KEY_FORWARD			MLX_KEY_W
# define KEY_LEFT				MLX_KEY_A
# define KEY_BACK				MLX_KEY_S
# define KEY_RIGHT				MLX_KEY_D
# define KEY_UP					MLX_KEY_SPACE
# define KEY_DOWN				MLX_KEY_LEFT_SHIFT

# define OBJ_HIDDEN_SCENE		(1 << 0) // norm issue, must only contain consts
# define OBJ_HIDDEN_CAM			(1 << 1)
# define OBJ_NO_CAST_SHADOW		(1 << 2)
# define MAT_NO_REC_SHADOW		(1 << 3)
# define MAT_DOUBLE_SIDED		(1 << 4)

typedef enum e_obj_type			t_obj_type;
typedef enum e_light_type		t_light_type;
typedef enum e_cam_state		t_cam_state;
typedef enum e_base_color		t_base_color;
typedef enum e_pattern			t_pattern;
typedef enum e_entity			t_entity;
typedef enum e_err_code			t_err_code;
typedef enum e_render_mode		t_render_mode;
typedef enum e_bn_channel		t_bn_channel;

typedef struct s_context		t_context;
typedef struct s_bvh_node		t_bvh_node;
typedef struct s_aabb			t_aabb;
typedef struct s_object			t_object;
typedef struct s_hit			t_hit;
typedef struct s_plane			t_plane;
typedef struct s_sphere			t_sphere;
typedef struct s_cylinder		t_cylinder;
typedef struct s_cone			t_cone;
typedef struct s_light			t_light;
typedef struct s_scene			t_scene;
typedef struct s_camera			t_camera;
typedef struct s_texture		t_texture;
typedef struct s_material		t_material;
typedef struct s_renderer		t_renderer;
typedef struct s_editor			t_editor;
typedef struct s_viewport		t_viewport;
typedef struct s_path			t_path;
typedef struct s_pixel			t_pixel;
typedef struct s_ray			t_ray;
typedef struct s_transform		t_transform;
typedef struct s_tex_entry		t_tex_entry;

typedef union u_shape			t_shape;

typedef mlx_image_t				t_image;

enum e_err_code
{
	ERR_ARGINVL,
	ERR_MLXINIT,
	ERR_IMGINIT,
	ERR_VECINIT,
	ERR_VECADD,
	ERR_RESIZE,
	ERR_SSIZE,
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
	ERR_TEXNPOT
};

enum e_obj_type
{
	OBJ_PLANE,
	OBJ_SPHERE,
	OBJ_CYLINDER,
	OBJ_CUBE,
	OBJ_CONE,
	OBJ_HYPERBOLOID,
	OBJ_PARABOLOID
};

enum e_light_type
{
	LIGHT_POINT,
	LIGHT_AMBIENT,
	LIGHT_AREA,
	LIGHT_DIRECTIONAL
};

enum e_cam_state
{
	CAM_DEFAULT,
	CAM_ZOOM,
	CAM_PAN,
	CAM_ORBIT,
	CAM_MOVE
};

enum e_base_color
{
	BASE_COLOR,
	BASE_PATTERN,
	BASE_TEXTURE
};

enum e_pattern
{
	PAT_NONE,
	PAT_CHECKERBOARD
};

enum e_entity
{
	ENT_CAMERA,
	ENT_LIGHT,
	ENT_OBJECT
};

enum e_render_mode
{
	RENDER_REFINE,
	RENDER_PREVIEW,
	RENDER_EDIT
};

enum e_bn_channel
{
	BN_PX_U,
	BN_PX_V,
	BN_SC_U,
	BN_SC_V,
	BN_CO_U,
	BN_CO_V,
	BN_LI,
	BN_PP_R,
	BN_PP_G,
	BN_PP_B
};

struct __attribute__((aligned(16))) s_transform
{
	t_vec3	pos;
	t_vec4	rot;
	t_vec3	scale;
};

struct __attribute__((aligned(16))) s_ray
{
	t_vec3		origin;
	t_vec3		dir;
	t_vec3		dir_recip;
	t_v4ui		signs;
};

struct __attribute__((aligned(16))) s_hit
{
	t_vec3			point;
	t_vec3			normal;
	t_vec2			uv;
	const t_object	*obj;
	float			t;
	bool			is_primary;
};

struct __attribute__((aligned(16))) s_texture
{
	mlx_texture_t	*tex;
	float			*pixels;
	uint32_t		width;
	uint32_t		height;
};

struct __attribute__((aligned(16))) s_material
{
	t_texture		texture;
	t_texture		normal_map;
	t_vec3			albedo;
	t_vec3			albedo2;
	t_vec3			emission;
	float			metallic;
	float			roughness;
	float			ior;
	float			transmission;
	float			pattern_scale;
	float			bump_strength;
	uint32_t		flags;
	t_base_color	base_color;
	t_pattern		pattern;
	bool			is_emissive;
};

struct __attribute__((aligned(16))) s_plane
{
	t_vec3			point;
	t_vec3			normal;
};

struct __attribute__((aligned(16))) s_sphere
{
	t_vec3			center;
	float			radius;
	float			radius_sq;
};

struct __attribute__((aligned(16))) s_cylinder
{
	t_vec3			axis;
	t_vec3			center;
	float			radius;
	float			height;
};

struct __attribute__((aligned(16))) s_cone
{
	t_vec3			apex;
	t_vec3			axis;
	float			angle;
	float			height;
	float			cos_sq;
	float			tan_sq;
	float			base_radius;
	float			inv_height;
};

union __attribute__((aligned(16))) u_shape
{
	t_plane			plane;
	t_sphere		sphere;
	t_cylinder		cylinder;
	t_cone			cone;
};

struct __attribute__((aligned(16))) s_object
{
	t_transform		transform;
	t_shape			shape;
	t_vec3			bounds_center;
	t_material		*mat;
	uint32_t		material_id;
	uint32_t		flags;
	t_obj_type		type;
};

struct __attribute__((aligned(16))) s_light
{
	union
	{
		t_vec3		pos;
		t_vec3		dir;
	};
	t_vec3			color;
	t_object		*obj;
	t_material		*mat;
	float			radius;
	float			intensity;
	float			max_brightness;
	uint32_t		material_id;
	t_light_type	type;
};

struct __attribute__((aligned(16))) s_viewport
{
	t_vec3			d_u;
	t_vec3			d_v;
	t_vec3			pixel_00_loc;
	float			width;
	float			height;
};

struct __attribute__((aligned(16))) s_camera
{
	t_viewport		viewport;
	t_light			directional_light;
	t_transform		transform;
	t_transform		target;
	t_vec3			pivot;
	t_vec3			up;
	t_vec3			right;
	t_vec3			forward;
	t_vec2			skydome_uv_offset;
	float			aspect;
	float			focal_length;
	float			fov;
	float			pitch;
	float			yaw;
	float			distance;
	float			exposure;
	t_cam_state		state;
};

struct __attribute__((aligned(16))) s_path
{
	t_ray			ray;
	t_hit			hit;
	t_vec3			color;
	t_vec3			throughput;
	t_vec3			f0;
	t_vec3			n;
	t_vec3			l;
	t_vec3			v;
	t_vec3			h;
	t_vec2			uv;
	t_material		*mat;
	int32_t			bounce;
	float			ndotl;
	float			ndotv;
	float			ndoth;
	float			vdoth;
	float			ldoth;
	float			alpha;
	float			pdf;
	float			p_spec;
	bool			sample_spec;
};

struct s_tex_entry
{
	char		name[MAX_NAME_LEN];
	t_texture	texture;
	bool		loaded;
};

struct __attribute__((aligned(16))) s_scene
{
	t_camera		cam;
	t_vector		objs;
	t_vector		lights;
	t_vector		materials;
	t_bvh_node		*bvh_root;
	t_light			ambient_light;
	t_texture		skydome;
	t_object		*selected_obj;
	int				tex_count;
	bool			has_directional_light;
	t_tex_entry		textures[MAX_TEXTURES];
};

struct __attribute__((aligned(64))) s_renderer
{
	struct __attribute__((aligned(64)))
	{
		pthread_mutex_t		mutex;
		pthread_cond_t		cond;
		uint32_t			tile_index;
		uint32_t			threads_running;
		uint32_t			tiles_total;
		bool				active;
		bool				resize_pending;
		bool				frame_complete;
	};
	struct __attribute__((aligned(64)))
	{
		t_vec3				*buffer;
		t_camera			cam;
		t_int2				tiles;
		uint32_t			width;
		uint32_t			height;
		uint32_t			pixels;
		t_render_mode		mode;
		uint32_t			render_samples; //
		uint32_t			refine_bounces;
		uint32_t			frame;
		uint8_t				ray_bounces;
		_Atomic bool		render_cancel;
	};
	struct __attribute__((aligned(64)))
	{
		pthread_t			*threads;
		uint32_t			new_width;
		uint32_t			new_height;
		int32_t				threads_amount;
		int32_t				threads_init;
		uint32_t			render_time;
		uint32_t			blit_time;
		bool				init_mutex;
		bool				init_cond;
	};
};

struct __attribute__((aligned(16))) s_editor
{
	t_object		selection;
};

struct __attribute__((aligned(16))) s_pixel
{
	t_vec3			*color;
	uint32_t		*seed;
	uint32_t		x;
	uint32_t		y;
	uint32_t		frame;
	float			u;
	float			v;
	float			scale;
};

struct __attribute__((aligned(16))) s_aabb
{
	t_vec3			min;
	t_vec3			max;
};

struct __attribute__((aligned(16))) s_bvh_node
{
	t_aabb			aabb;
	t_bvh_node		*left;
	t_bvh_node		*right;
	t_object		*obj;
	int				axis;
};

struct __attribute__((aligned(64))) s_context
{
	t_renderer		renderer;
	t_scene			scene;
	t_texture		tex_bn;
	t_editor		editor;
	mlx_t			*mlx;
	t_image			*img;
	uint32_t		resize_time;
	int				fd;
};

#endif
