#ifndef PARSING_H
# define PARSING_H

# include "defines.h"

typedef enum e_parse_error
{
	PARSE_OK = 0,           /* Successfully parsed                          */
	PARSE_ERR_EMPTY,        /* Empty or whitespace-only line (skip it)      */
	PARSE_ERR_UNKNOWN_ID,   /* Unknown element identifier                   */
	PARSE_ERR_MISSING_ARGS, /* Not enough arguments for element             */
	PARSE_ERR_INVALID_NUM,  /* Could not parse number                       */
	PARSE_ERR_RANGE,        /* Value out of valid range                     */
	PARSE_ERR_DUPLICATE,    /* Duplicate unique element (A, C, or L)        */
	PARSE_ERR_MALLOC        /* Memory allocation failed                     */
}				t_parse_error;

typedef struct s_parser
{
	int line_num; // current line number
	bool		has_light;
	bool		has_ambient;
	bool		has_camera;
}				t_parser;

bool			parse_scene(t_context *ctx, int fd);
bool			parse_float(char *str, float *out);
bool			parse_color(char *str, t_color *color);
bool			parse_vec3(char *str, t_vec3 *vec);

bool			validate_range(float value, float min, float max);
bool			validate_normalized(t_vec3 vec);
int				count_tokens(char **tokens);
void			free_tokens(char **tokens);

t_parse_error	init_plane(t_context *ctx, t_vec3 point, t_vec3 normal, t_material *mat);

t_parse_error	parse_plane(t_context *ctx, char **tokens);
t_parse_error	parse_sphere(t_context *ctx, char **tokens);
t_parse_error	parse_ambient(t_context *ctx, t_parser *p, char **tokens);
t_parse_error	parse_light(t_context *ctx, t_parser *p, char **tokens);
t_parse_error	parse_camera(t_context *ctx, t_parser *p, char **tokens);

#endif