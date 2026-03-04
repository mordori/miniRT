#ifndef MINIRT_MATERIALS_H
# define MINIRT_MATERIALS_H

# include "defines.h"

uint32_t	new_material(t_context *ctx, t_material *mat);
void	set_material_data(t_path *path);
void	set_shader_data(t_path *path);

/* Texture sampling */
t_vec3		sample_texture(const t_texture *tex, t_vec2 uv);
t_vec3		get_surface_color(const t_material *mat, const t_hit *hit);

/*Procedural patterns*/
t_vec3		eval_pattern(t_material *mat, t_hit *hit);
float		perlin_noise(float x, float y, float z);
float		turbulence(t_vec3 p);

t_vec3		pattern_perlin_marble(t_hit *hit, const t_material *mat);
t_vec3		pattern_perlin_wood(t_hit *hit, const t_material *mat);
t_vec3		pattern_perlin_turb(t_hit *hit, const t_material *mat);
t_vec3		compute_bump_offset(t_vec3 p, float strength);



#endif
