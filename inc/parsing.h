#ifndef PARSING_H
# define PARSING_H

# include "defines.h"

# define MAX_NAME_LEN		64
# define MAX_TEXTURES		64
# define MAX_MATERIALS		64
# define TEXTURE_PATH		"assets/textures/"

typedef enum e_error
{
	PARSE_OK = 0,
	PARSE_ERR_EMPTY,
	PARSE_ERR_UNKNOWN_ID,
	PARSE_ERR_MISSING_ARGS,
	PARSE_ERR_INVALID_NUM,
	PARSE_ERR_RANGE,
	PARSE_ERR_DUPLICATE,
	PARSE_ERR_MALLOC,
	PARSE_ERR_MISSING_OBJ,
	PARSE_ERR_TEXTURE,
	PARSE_ERR_MATERIAL,
	PARSE_ERR_TOO_MANY
}				t_error;

typedef struct s_tex_entry
{
	char		name[MAX_NAME_LEN];
	t_texture	texture;
	bool		loaded;
}	t_tex_entry;

typedef struct s_mat_entry
{
	t_material	material;
	bool		defined;
}	t_mat_entry;

typedef struct s_parser
{
	int				line_num;
	bool			has_light;
	bool			has_ambient;
	bool			has_camera;
	bool			has_sphere;
	bool			has_plane;
	bool			has_cylinder;
	t_tex_entry		textures[MAX_TEXTURES];
	t_mat_entry		materials[MAX_MATERIALS];
	int				tex_count;
	int				mat_count;
}				t_parser;

/* Main entry point */
bool			parse_scene(t_context *ctx, int fd);

/* Basic parsing utilities */
bool			parse_float(char *str, float *out);
bool			parse_uint(char *str, uint32_t *out);
bool			parse_int(char *str, int *out);
bool			parse_color(char *str, t_vec3 *color);
bool			parse_vec3(char *str, t_vec3 *vec);
int				count_tokens(char **tokens);
void			free_tokens(char **tokens);

/* Validation helpers */
bool			validate_range(float value, float min, float max);
bool			validate_normalized(t_vec3 vec);

/* Element parsing */
t_error			parse_sphere(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_plane(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_cylinder(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_ambient(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_skydome(t_context *ctx, char **tokens);
t_error			parse_light(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_camera(t_context *ctx, t_parser *p, char **tokens);

/* Material and texture parsing */
// t_error			parse_material_def(t_parser *p, char **tokens);
t_error			parse_material_def(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_material_token(t_parser *p, const char *token,
					t_material *out);
t_material		*get_material_by_id(t_parser *p, int id);

t_error			parse_texture_def(t_context *ctx, t_parser *p, char **tokens);
t_error			load_texture_file(t_context *ctx, const char *filename,
					t_texture *out);
t_texture		*find_texture_by_name(t_parser *p, const char *name);
bool			is_placeholder(const char *str);

/* Area light from emissive objects */
t_error			create_area_light(t_context *ctx, t_object *obj,
					t_material *mat);

/* Cleanup */
void			cleanup_parser(t_parser *p);

#endif
