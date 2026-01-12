#ifndef MINIRT_DEFINES_H
# define MINIRT_DEFINES_H

# include <pthread.h>
# include <stdatomic.h>

# include "MLX42.h"
# include "libft_defs.h"
# include "libft_vector.h"
# include "libft_colors.h"
# include "libft_graphics.h"

# define WIDTH			1920
# define HEIGHT			1080
# define THREADS_DFL	4
# define CPU_SPIN		150
# define RENDER_PASSES	4
# define SENS_ORBIT		0.0025f
# define SENS_ZOOM		0.0018f
# define SENS_PAN		0.0006f
# define TILE_SIZE		32
# define INV_255F		0.003921568627451f


# ifndef M_1_2PI
#  define M_1_2PI		0.15915494309189533577f
# endif

# ifndef G_EPSILON
#  define G_EPSILON		1e-3f
# endif

# ifndef SHADOW_BIAS
#  define SHADOW_BIAS	1e-4f
# endif

# define OBJ_VISIBLE			(1 << 0)
# define OBJ_CAST_SHADOWS		(1 << 1)

# define MAT_DOUBLE_SIDED		(1 << 0)
# define MAT_RECEIVE_SHADOWS	(1 << 1)

typedef enum e_obj_type		t_obj_type;
typedef enum e_light_type	t_light_type;
typedef enum e_cam_state	t_cam_state;
typedef enum e_base_color	t_base_color;
typedef enum e_surface_type	t_surface_type;
typedef enum e_pattern		t_pattern;
typedef enum e_entity		t_entity;
typedef enum e_err_code		t_err_code;

typedef struct s_context	t_context;
typedef struct s_bvh_node	t_bvh_node;
typedef struct s_aabb		t_aabb;
typedef struct s_object		t_object;
typedef struct s_hit		t_hit;
typedef struct s_plane		t_plane;
typedef struct s_sphere		t_sphere;
typedef struct s_cylinder	t_cylinder;
typedef struct s_light		t_light;
typedef struct s_scene		t_scene;
typedef struct s_camera		t_camera;
typedef struct s_texture	t_texture;
typedef struct s_material	t_material;
typedef struct s_renderer	t_renderer;
typedef struct s_editor		t_editor;
typedef struct s_viewport	t_viewport;

typedef union u_shape		t_shape;

typedef mlx_image_t			t_image;

typedef void				(*t_add_entity)(t_context *, char **);
typedef t_aabb				(*t_get_shape_bounds)(const t_object *);
typedef bool				(*t_hit_shape)(const t_shape *, const t_ray *, t_hit *);

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
	ERR_MATADD
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
	LIGHT_DIRECTIONAL
};

enum e_cam_state
{
	CAM_DEFAULT,
	CAM_ZOOM,
	CAM_PAN,
	CAM_ORBIT
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

enum e_surface_type
{
	SURF_OPAQUE,
	SURF_TRANSPARENT
};

enum e_entity
{
	ENT_CAMERA,
	ENT_LIGHT,
	ENT_OBJECT
};

struct s_hit
{
	t_vec3			point;
	t_vec3			normal;
	t_vec2			uv;
	const t_object	*obj;
	float			t;
};

struct s_texture
{
	mlx_texture_t	*tex;
	float			*pixels;
	uint32_t		width;
	uint32_t		height;
};

struct __attribute__((aligned(16))) s_material
{
	t_vec4			color;
	t_texture		texture;
	t_texture		normal_map;
	float			alpha;
	t_base_color	base_color;
	t_surface_type	surface_type;
	t_pattern		pattern;
	uint32_t		flags;
};

struct __attribute__((aligned(32))) s_plane
{
	t_vec3			point;
	t_vec3			normal;				//
};

struct __attribute__((aligned(32))) s_sphere
{
	t_vec3			center;
	float			radius;
	float			radius_squared;
};

struct __attribute__((aligned(32))) s_cylinder
{
	float			radius;
	float			height;
};

union __attribute__((aligned(32))) u_shape
{
	t_plane			plane;
	t_sphere		sphere;
	t_cylinder		cylinder;
};

struct __attribute__((aligned(16))) s_object
{
	t_transform		transform;
	t_shape			shape;
	t_vec3			bounds_center;
	t_vec2			uv;
	t_obj_type		type;
	uint32_t		material_id;
	uint32_t		flags;
};

struct __attribute__((aligned(16))) s_light
{
	t_vec3			pos_dir;
	t_vec4			color;
	float			radius;
	float			intensity;
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
	t_transform		transform;
	t_transform		target;
	t_vec3			pivot;
	t_vec3			up;
	t_vec3			right;
	t_vec3			forward;
	float			aspect;
	float			focal_length;
	float			fov;
	float			pitch;
	float			yaw;
	float			distance;
	t_cam_state		state;
};

struct s_scene
{
	t_camera		cam;
	t_vector		objs;
	t_vector		lights;
	t_vector		materials;
	t_texture		skydome;
	t_light			ambient_light;
	t_light			directional_light;
	t_bvh_node		*bvh_root;
	t_object		*selected_obj;
};

struct s_renderer
{
	pthread_mutex_t		mutex;
	uint32_t			tile_index;
	uint32_t			threads_running;
	uint32_t			tiles_total;
	bool				active;
	bool				resize_pending;
	bool				frame_complete;
	uint8_t				padding_0[9];
	pthread_cond_t		cond;
	uint8_t				padding_1[16];
	t_vec3				*buffer_a;
	t_vec3				*buffer_b;
	uint32_t			width;
	uint32_t			height;
	uint32_t			pixels;
	t_int2				tiles;
	uint8_t				padding_2[28];
	t_camera			cam;
	pthread_t			*threads;
	uint32_t			new_width;
	uint32_t			new_height;
	int32_t				threads_amount;
	int32_t				threads_init;
	bool				init_mutex;
	bool				init_cond;
};

struct s_editor
{

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

struct s_context
{
	t_renderer		renderer;
	t_scene			scene;
	t_editor		editor;
	mlx_t			*mlx;
	t_image			*img;
	uint32_t		resize_time;
	int				fd;
};

#endif
