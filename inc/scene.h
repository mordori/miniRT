#ifndef SCENE_H
#define SCENE_H

#include "defines.h"

void validate_file_type(char* file);
void init_scene(t_context* ctx);
void clean_scene(t_context* ctx);

bool init_bvh(t_context* ctx);
bool hit_bvh(uint32_t root_idx, const t_ray* ray, t_hit* hit, t_bvh_node* tree);
bool hit_bvh_editing(uint32_t root_idx, const t_ray* ray, t_hit* hit, t_bvh_node* tree);
bool hit_bvh_shadow(uint32_t root_idx, const t_ray* ray, float dist, t_bvh_node* tree);
bool hit_aabb(const t_aabb* aabb, const t_ray* ray, float closest_t);

void init_mesh_bvh(t_context* ctx, t_mesh* mesh);
bool hit_bvh_mesh(const t_mesh* mesh, const t_ray* ray, t_hit* hit);

t_aabb get_volume_bounds(t_object** objs, size_t n);
t_aabb get_mesh_volume_bounds(t_triangle* tris, size_t start, size_t end);
t_aabb get_object_bounds(const t_object* obj);

int cmp_bounds_x(const void* a, const void* b);
int cmp_bounds_y(const void* a, const void* b);
int cmp_bounds_z(const void* a, const void* b);

int cmp_tri_x(const void* a, const void* b);
int cmp_tri_y(const void* a, const void* b);
int cmp_tri_z(const void* a, const void* b);

void update_bounds(t_object* obj);
float get_max_bounds_dim(const t_object* obj);

#endif
