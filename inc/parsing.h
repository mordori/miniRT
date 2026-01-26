#ifndef PARSING_H
# define PARSING_H

# include "defines.h"

# define MAX_NAME_LEN 64
# define MAX_TEXTURES 64
# define MAX_MATERIALS 64
# define TEXTURE_PATH		"assets/textures/"


typedef enum e_error
{
	PARSE_OK = 0,           /* Successfully parsed                          */
	PARSE_ERR_EMPTY,        /* Empty or whitespace-only line (skip it)      */
	PARSE_ERR_UNKNOWN_ID,   /* Unknown element identifier                   */
	PARSE_ERR_MISSING_ARGS, /* Not enough arguments for element             */
	PARSE_ERR_INVALID_NUM,  /* Could not parse number                       */
	PARSE_ERR_RANGE,        /* Value out of valid range                     */
	PARSE_ERR_DUPLICATE,    /* Duplicate unique element (A, C, or L)        */
	PARSE_ERR_MALLOC,       /* Memory allocation failed                     */
	PARSE_ERR_MISSING_OBJ,  /* No objects defined in the scene              */
	PARSE_ERR_TEXTURE,
	PARSE_ERR_MATERIAL,
	PARSE_ERR_TOO_MANY		/* Too many textures or materials loaded        */
}				t_error;

/**
 * Named texture - allows textures to be loaded and stored.
 */
typedef struct s_tex
{
	char		name[MAX_NAME_LEN]; // lookup by name
	t_texture	texture;
	bool		loaded;
}	t_tex;

/**
 * Material entry stored by ID.
 * Materials are referenced by index in the materials array.
 */
typedef struct s_mat
{
	// char		name[MAX_MATERIALS];
	t_material	material;
	bool		defined;
}	t_mat;

typedef struct s_parser
{
	bool		has_light;
	bool		has_ambient;
	bool		has_camera;
	bool		has_sphere;
	bool		has_plane;
	bool		has_cylinder;
	t_mat		*materials;
	t_tex		*textures;
	int			material_count;
	int			texture_count;
	int 		line_num;
}				t_parser;

bool			parse_scene(t_context *ctx, int fd);
bool			parse_float(char *str, float *out);
bool			parse_color(char *str, t_vec3 *color);
bool			parse_vec3(char *str, t_vec3 *vec);

bool			validate_range(float value, float min, float max);
bool			validate_normalized(t_vec3 vec);
int				count_tokens(char **tokens);
void			free_tokens(char **tokens);

t_error	init_plane(t_context *ctx, t_vec3 point, t_vec3 normal, t_material *mat);
t_error	init_cylinder(t_context *ctx, t_cylinder *cy, t_material *mat);
// t_error	init_sphere(t_context *ctx, t_vec3 center, float diameter, t_material *mat);

t_error	parse_sphere(t_context *ctx, t_parser *parser, char **tokens);
t_error	parse_plane(t_context *ctx, t_parser *parser, char **tokens);
t_error	parse_cylinder(t_context *ctx, t_parser *parser, char **tokens);

t_error	parse_ambient(t_context *ctx, t_parser *p, char **tokens);
t_error	parse_light(t_context *ctx, t_parser *p, char **tokens);
t_error	parse_camera(t_context *ctx, t_parser *p, char **tokens);
t_error	parse_skydome(t_context *ctx, char **tkns);

t_texture		*find_texture_name(t_parser *p, const char *name);
t_error	parse_texture(t_context *ctx, t_parser *p, char **tkns);
t_error	load_texture_file(t_context *ctx, const char *filename, t_texture *out);

bool			is_png_filename(const char *str);
void			cleanup_parser(t_parser *p);
#endif
