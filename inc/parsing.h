#ifndef PARSING_H
# define PARSING_H

# include "defines.h"

# define MAX_MATERIALS 64
# define MAX_LINES 1024
# define TEXTURE_PATH "assets/textures/"

typedef enum e_error
{
	E_OK = 0,
	E_EMPTY,
	E_UNKNOWN_ID,
	E_MISSING_ARGS,
	E_INVALID_NUM,
	E_RANGE,
	E_DUPLICATE,
	E_MALLOC,
	E_MISSING_OBJ,
	E_TEXTURE,
	E_MATERIAL,
	E_EMISSIVE,
	E_TOO_MANY
}				t_error;

typedef struct s_mat_entry
{
	t_material	material;
	bool		defined;
}				t_mat_entry;

typedef struct s_parser
{
	int			line_num;
	bool		has_light;
	bool		has_ambient;
	bool		has_camera;
	bool		has_sphere;
	bool		has_plane;
	bool		has_cylinder;
	bool		has_cone;
	t_mat_entry	materials[MAX_MATERIALS];
	uint32_t	mat_count;
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
int				count_delimiter(const char *str, char delim);

void			free_tokens(char **tokens);
void			try_free_all(char **lines, int count);

/* Validation helpers */
void			validate_scene(t_context *ctx, t_parser *p);

bool			validate_range(float value, float min, float max);
bool			validate_normalized(t_vec3 vec);

/* Element parsing */
t_error			parse_sphere(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_plane(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_cylinder(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_cone(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_ambient(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_skydome(t_context *ctx, char **tokens);
t_error			parse_light(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_camera(t_context *ctx, t_parser *p, char **tokens);
t_error			identify_element(t_context *ctx, t_parser *p, char **tokens);

/* Material and texture parsing */
t_error			parse_material_def(t_context *ctx, t_parser *p, char **tokens);
t_error			parse_material_token(t_parser *p, const char *token,
					t_material *out);
t_error			resolve_material(t_context *ctx, t_parser *p, const char *token,
					uint32_t *out_id);
t_material		*get_material_by_id(t_parser *p, uint32_t id);
void			try_pass(t_context *ctx, t_parser *p, char **lines, int pass);
t_error			dispatch_pass(t_context *ctx, t_parser *p, char **tokens,
					int pass);
bool			parse_pattern_token(const char *tkn, t_pattern *out);
t_error 		parse_mat_pattern(t_material *mat, char **tkns, int tc);

t_error			try_render_settings(t_context *ctx, char **tokens);
t_error			parse_texture_def(t_context *ctx, char **tokens);
t_error			load_texture_file(const char *filename, t_texture *out);
t_texture		*find_texture_by_name(t_scene *scene, const char *name);
bool			is_placeholder(const char *str);
void			print_error(t_context *ctx, t_error err, int line_num);

#endif
