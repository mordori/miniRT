#ifndef MINIRT_DEFINES_H
# define MINIRT_DEFINES_H

# include <pthread.h>

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
# define RENDER_TILE	8

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
typedef struct s_hit		t_hit;
typedef struct s_object		t_object;
typedef struct s_plane		t_plane;
typedef struct s_sphere		t_sphere;
typedef struct s_cylinder	t_cylinder;
typedef struct s_light		t_light;
typedef struct s_scene		t_scene;
typedef struct s_camera		t_camera;
typedef struct s_material	t_material;
typedef struct s_renderer	t_renderer;
typedef struct s_editor		t_editor;
typedef struct s_viewport	t_viewport;

typedef union u_shape		t_shape;

typedef mlx_image_t			t_image;
typedef mlx_texture_t		t_texture;

typedef void				(*t_add_entity)(t_context *, char **);

enum e_err_code
{
	ERR_ARGINVL,
	ERR_MLXINIT,
	ERR_IMGINIT,
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

struct s_material
{
	t_base_color	base_color;
	t_surface_type	surface_type;
	t_color			color;
	t_pattern		pattern;
	t_texture		*texture;
	t_texture		normal_map;
	float			alpha;
	bool			double_sided;
	bool			cast_shadows;
	bool			receive_shadows;
};

struct s_plane
{
	t_vec3			dimensions;
};

struct s_sphere
{
	t_vec3			center;
	float			radius;
	float			radius_squared;
};

struct s_cylinder
{
	float			radius;
	float			height;
};

union u_shape
{
	t_plane			plane;
	t_sphere		sphere;
	t_cylinder		cylinder;
};

struct s_object
{
	t_transform		transform;
	t_material		material;
	t_float2		uv;
	t_obj_type		type;
	t_shape			shape;
};

struct s_light
{
	t_transform		transform;
	t_light_type	type;
	t_vec3			direction;
	float			radius;
	float			intensity;
	t_color			color;
};

struct s_viewport
{
	float	width;
	float	height;
	t_vec3	u;
	t_vec3	v;
};

struct s_camera
{
	t_transform		transform;
	t_transform		target;
	t_vec3			pivot;
	t_vec3			up;
	t_vec3			right;
	float			aspect;
	float			focal_length;
	float			fov;
	float			pitch;
	float			yaw;
	float			roll;
	float			distance;
	t_cam_state		state;
	t_viewport		viewport;
};

struct s_scene
{
	t_camera		cam;
	t_vector		objs;
	t_bvh_node		*bvh_root;
	t_vector		lights;
	t_texture		*skydome;
	t_light			ambient_light;
	t_light			directional_light;
};

struct s_renderer
{
	uint32_t		jobs;
	pthread_t		*threads;
	long			threads_init;
	long			threads_amount;
	_Atomic bool	active;
	_Atomic bool	paused;
	_Atomic bool	finished;
	uint8_t			pass;
};

struct s_editor
{
	t_object		*selected_object;
};

struct s_hit
{
	float			t;
	t_vec3			point;
	t_vec3			normal;
	t_vec4			color;
	bool			front_face;
};

struct s_aabb
{
	t_vec3			min;
	t_vec3			max;
};

struct s_bvh_node
{
	t_aabb			box;
	t_bvh_node		*left;
	t_bvh_node		*right;
	t_object		*obj;
};

struct s_context
{
	mlx_t			*mlx;
	t_image			*img;
	t_scene			scene;
	t_editor		editor;
	t_renderer		renderer;
};

#endif
