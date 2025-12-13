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
	ERR_BVH
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

struct s_texture
{
	mlx_texture_t	*tex;
	float			*pixels;
	uint32_t		width;
	uint32_t		height;
};

struct s_material
{
	t_vec4			color;
	t_texture		texture;
	t_texture		normal_map;
	float			alpha;
	t_base_color	base_color;
	t_surface_type	surface_type;
	t_pattern		pattern;
	bool			double_sided;
	bool			cast_shadows;
	bool			receive_shadows;
	uint8_t			padding[13];		// Optimizes to 64 bytes
};

struct s_plane
{
	t_vec3			dimensions;
	uint8_t			padding[16];		// Optimizes to 32 bytes
};

struct s_sphere
{
	t_vec3			center;
	float			radius;
	float			radius_squared;
	uint8_t			padding[8];			// Optimizes to 32 bytes
};

struct s_cylinder
{
	float			radius;
	float			height;
	// Adjust padding later
	uint8_t			padding[24];		// Optimizes to 32 bytes
};

union u_shape
{
	t_plane			plane;
	t_sphere		sphere;
	t_cylinder		cylinder;			// 32 bytes
};

struct s_object
{
	t_material		material;
	t_transform		transform;
	t_shape			shape;
	t_vec3			bounds_center;
	t_vec2			uv;
	t_obj_type		type;
	uint8_t			padding[4];			// Optimizes to 176 bytes
};

struct s_light
{
	t_transform		transform;
	t_vec3			direction;
	t_vec4			color;
	float			radius;
	float			intensity;
	t_light_type	type;
	uint8_t			padding[4];			// Optimizes to 96 bytes
};

struct s_viewport
{
	t_vec3			d_u;
	t_vec3			d_v;
	t_vec3			pixel_00_loc;
	float			width;
	float			height;
	uint8_t			padding[8];			// Optimizes to 64 bytes
};

struct s_camera
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
	t_cam_state		state;				// 260 bytes -- Adjust later
};

struct s_scene
{
	t_camera		cam;
	t_vector		objs;
	t_vector		lights;
	t_bvh_node		*bvh_root;
	t_texture		skydome;
	t_light			ambient_light;
	t_light			directional_light;	// 596 bytes -- Adjust later
};

struct s_renderer
{
	_Atomic uint32_t	tile_index;			// High contention writes
	_Atomic uint32_t	tiles_done;
	uint8_t				padding_1[56];		// Optimizes to 64 bytes
	t_vec3				*buffer;			// Frequent reads, singular writes
	pthread_t			*threads;
	long				threads_init;
	long				threads_amount;
	t_int2				tiles;
	uint32_t			width;
	uint32_t			height;
	uint32_t			pixels;
	uint32_t			tiles_total;
	_Atomic bool		finished;
	_Atomic bool		resize_pending;
	_Atomic bool		active;
	// uint8_t				padding_2[17];		// Optimizes to 64 bytes
};

struct s_editor
{
	t_object		*selected_object;
};

struct s_aabb
{
	t_vec3			min;
	t_vec3			max;				// 32 bytes
};

struct s_bvh_node
{
	t_aabb			aabb;
	t_bvh_node		*left;
	t_bvh_node		*right;
	t_object		*obj;
	int				axis;
	uint8_t			padding[4];			// Optimizes to 64 bytes
};

struct s_context
{
	t_renderer		renderer;
	t_scene			scene;
	t_editor		editor;
	mlx_t			*mlx;
	t_image			*img;
	int				fd;
};

#endif
