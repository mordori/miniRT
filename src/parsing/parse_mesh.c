#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "defines.h"
#include "lib_math.h"
#include "libft_utils.h"
#include "utils.h"

static inline void count_elements(char* ptr, char* end, uint32_t* v, uint32_t* vn, uint32_t* vt, uint32_t* tris);
static inline void parse_vec3(char** ptr, t_vec3* vec);
static inline void parse_vec2(char** ptr, t_vec2* vec);

static inline void parse_face(char** ptr, char* end, uint32_t* tri_i, t_triangle* triangles, t_vec3* verts, t_vec3* normals, t_vec2* uvs,
	uint32_t v_count, uint32_t vn_count, uint32_t vt_count);
static inline void parse_vertex(char** ptr, int32_t* v, int32_t* uv, int32_t* n);
static inline uint32_t get_index(int32_t i, uint32_t count);

void parse_obj(t_context* ctx, const char* file, t_mesh* mesh) {
	int fd = open(file, O_RDONLY);
	if (fd < 0)
		return;

	struct stat sb;
	if (fstat(fd, &sb) < 0) {
		close(fd);
		return;
	}

	char* data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (data == MAP_FAILED) {
		close(fd);
		return;
	}

	char* ptr = data;
	char* end = data + sb.st_size;
	t_vec3* verts;
	t_vec3* normals;
	t_vec2* uvs;
	{
		uint32_t v = 0, vn = 0, vt = 0, tris = 0;
		count_elements(ptr, end, &v, &vn, &vt, &tris);

		if (v == 0 || tris == 0) {
			munmap(data, sb.st_size);
			close(fd);
			mesh->triangle_count = 0;
			mesh->triangles = NULL;
			printf("Error: Invalid mesh: %s\n", file);
			return;
		}

		verts = malloc(sizeof(t_vec3) * v);
		normals = malloc(sizeof(t_vec3) * vn);
		uvs = malloc(sizeof(t_vec2) * vt);
		mesh->triangle_count = tris;
		mesh->triangles = malloc(sizeof(t_triangle) * tris);
		if (!mesh->triangles || !verts || (vn > 0 && !normals) || (vt > 0 && !uvs))
			fatal_error(ctx, errors(ERR_MALLOC), __FILE__, __LINE__);
	}
	{
		uint32_t v_i = 0, vn_i = 0, vt_i = 0, tri_i = 0;
		while (ptr < end) {
			char* line = memchr(ptr, '\n', end - ptr);
			if (!line)
				break;
			if (*ptr == 'v' && *(ptr + 1) == ' ') {
				ptr += 2;
				parse_vec3(&ptr, &verts[v_i]);
				++v_i;
			} else if (*ptr == 'v' && *(ptr + 1) == 'n') {
				ptr += 3;
				parse_vec3(&ptr, &normals[vn_i]);
				++vn_i;
			} else if (*ptr == 'v' && *(ptr + 1) == 't') {
				ptr += 3;
				parse_vec2(&ptr, &uvs[vt_i]);
				++vt_i;
			} else if (*ptr == 'f' && *(ptr + 1) == ' ') {
				ptr += 2;
				parse_face(&ptr, end, &tri_i, mesh->triangles, verts, normals, uvs, v_i, vn_i, vt_i);
			}
			ptr = line + 1;
		}
	}
	free(verts);
	free(normals);
	free(uvs);
	munmap(data, sb.st_size);
	close(fd);
}

static inline void count_elements(char* ptr, char* end, uint32_t* v, uint32_t* vn, uint32_t* vt, uint32_t* tris) {
	while (ptr < end) {
		if (*ptr == 'v') {
			switch (*(ptr + 1)) {
				case ' ': ++(*v); break;
				case 'n': ++(*vn); break;
				case 't': ++(*vt); break;
				default: break;
			}
		} else if (*ptr == 'f' && *(ptr + 1) == ' ') {
			int verts = 0;
			char* f_ptr = ptr + 2;
			while (*f_ptr != '\n' && *f_ptr != '\r' && f_ptr < end) {
				if (*f_ptr != ' ' && *f_ptr != '\t') {
					++verts;
					while (!ft_isspace(*f_ptr) && f_ptr < end)
						++f_ptr;
				} else {
					++f_ptr;
				}
			}
			if (verts >= 3)
				*tris += verts - 2;
		}
		char* next_line = memchr(ptr, '\n', end - ptr);
		if (!next_line)
			break;
		ptr = next_line + 1;
	}
}

static inline void parse_vec3(char** ptr, t_vec3* vec) {
	vec->x = ft_atof(ptr);
	vec->y = ft_atof(ptr);
	vec->z = -ft_atof(ptr);
	vec->w = 0.0f;
}

static inline void parse_vec2(char** ptr, t_vec2* vec) {
	vec->u = ft_atof(ptr);
	vec->v = ft_atof(ptr);
}

static inline void parse_face(char** ptr, char* end, uint32_t* tri_i, t_triangle* triangles, t_vec3* verts, t_vec3* normals, t_vec2* uvs,
	uint32_t v_count, uint32_t vn_count, uint32_t vt_count) {
	int32_t v[3], n[3], uv[3];
	parse_vertex(ptr, &v[0], &uv[0], &n[0]);
	parse_vertex(ptr, &v[1], &uv[1], &n[1]);
	while (**ptr != '\n' && **ptr != '\r' && *ptr < end && **ptr) {
		parse_vertex(ptr, &v[2], &uv[2], &n[2]);
		t_triangle* tri = &triangles[*tri_i];
		tri->v0 = verts[get_index(v[0], v_count)];
		tri->v1 = verts[get_index(v[2], v_count)];
		tri->v2 = verts[get_index(v[1], v_count)];

		tri->has_uvs = false;
		if (uv[0] != 0) {
			tri->has_uvs = true;
			tri->uv0 = uvs[get_index(uv[0], vt_count)];
			tri->uv1 = uvs[get_index(uv[2], vt_count)];
			tri->uv2 = uvs[get_index(uv[1], vt_count)];
		}

		tri->has_normals = false;
		if (n[0] != 0) {
			tri->has_normals = true;
			tri->n0 = normals[get_index(n[0], vn_count)];
			tri->n1 = normals[get_index(n[2], vn_count)];
			tri->n2 = normals[get_index(n[1], vn_count)];
		}

		++(*tri_i);
		v[1] = v[2];
		uv[1] = uv[2];
		n[1] = n[2];
	}
}

static inline void parse_vertex(char** ptr, int32_t* v, int32_t* uv, int32_t* n) {
	*v = ft_atoi(ptr);
	*uv = 0;
	*n = 0;
	if (**ptr == '/') {
		++(*ptr);
		if (**ptr != '/')
			*uv = ft_atoi(ptr);
	}
	if (**ptr == '/') {
		++(*ptr);
		if (**ptr != '/')
			*n = ft_atoi(ptr);
	}
	while (**ptr == ' ' || **ptr == '\t')
		++(*ptr);
}

static inline uint32_t get_index(int32_t i, uint32_t count) {
	if (i < 0)
		return (uint32_t)(count + i);
	return (uint32_t)(i - 1);
}
