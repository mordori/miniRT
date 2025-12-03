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
# define SPIN_TIME		250

typedef enum e_obj_type		t_obj_type;
typedef enum e_light_type	t_light_type;
typedef enum e_cam_state	t_cam_state;
typedef enum e_base_color	t_base_color;
typedef enum e_surface_type	t_surface_type;
typedef enum e_pattern		t_pattern;
typedef enum e_entity_id	t_entity_id;

typedef struct s_context	t_context;
typedef struct s_object		t_object;
typedef struct s_light		t_light;
typedef struct s_scene		t_scene;
typedef struct s_camera		t_camera;
typedef struct s_material	t_material;
typedef struct s_entity		t_entity;

typedef mlx_texture_t		t_texture;

typedef void				(*t_add_entity)(t_context *, t_entity);

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

enum e_entity_id
{
	ENT_CAMERA,
	ENT_OBJECT,
	ENT_LIGHT,
	ENT_SKYDOME
};

struct s_entity
{
	t_entity_id		id;
	char			*tex_path;
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

struct s_object
{
	t_transform		transform;
	t_obj_type		type;
	t_material		material;
	t_float2		uv;
};

struct s_light
{
	t_transform		transform;
	t_light_type	type;
	float			radius;
	float			intensity;
	t_color			color;
};

struct s_camera
{
	t_transform		transform;
	t_transform		target;
	t_vec3			up;
	float			distance;
	float			fov;
	float			aspect;
	float			near;
	float			far;
	t_cam_state		state;
};

struct s_scene
{
	t_camera		cam;
	t_vector		objs;
	t_vector		lights;
	t_object		sky_sphere;
};

struct s_context
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_scene			scene;
	pthread_t		*threads;
	long			threads_init;
	long			threads_target;
	_Atomic bool	render_active;
	_Atomic bool	render_paused;
	_Atomic bool	render_finished;
};

#endif
