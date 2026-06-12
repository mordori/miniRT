#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdint.h>

#include "defines.h"
#include "parsing.h"

void load_mesh_dir(t_context* ctx, const char* dir_path);
void parse_obj(t_context* ctx, const char* file, t_mesh* mesh);
void add_mesh(t_context* ctx, const char* name, uint32_t mat_id, bool is_selected);
void add_sphere(t_context* ctx, uint32_t mat_id, bool is_selected);
bool del_object(t_context* ctx);
bool dup_object(t_context* ctx);
t_error add_object(t_context* ctx, t_object* obj, bool is_selected);

bool hit_mesh(const t_shape* shape, const t_ray* ray, t_hit* hit, uint32_t flags);
bool hit_object(const t_object* obj, const t_ray* ray, t_hit* hit);
bool hit_sphere(const t_shape* shape, const t_ray* ray, t_hit* hit);
bool hit_quad(const t_shape* shape, const t_ray* ray, t_hit* hit, uint32_t flags);
bool hit_triangle(const t_triangle* tri, const t_ray* ray, t_hit* hit, uint32_t flags);

bool test_body_hit(const t_ray* ray, float params[2], float t);
void update_transform(t_transform* t);

t_error init_sphere(t_context* ctx, t_vec3 center, float diameter, uint32_t mat_id);
t_error init_quad(t_context* ctx, t_quad* quad, uint32_t mat_id);

#endif
