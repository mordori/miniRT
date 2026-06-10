#include <dirent.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "libft_vector.h"
#include "objects.h"
#include "scene.h"
#include "utils.h"

static inline bool is_obj_file(const char* file);

void load_mesh_dir(t_context* ctx, const char* dir_path) {
	DIR* dir = opendir(dir_path);
	if (!dir) {
		printf("Warning: Could not open directory: %s\n", dir_path);
		return;
	}

	char file_path[512];
	struct dirent* entry;
	while ((entry = readdir(dir))) {
		if (entry->d_name[0] == '.')
			continue;

		if (is_obj_file(entry->d_name)) {
			if (ctx->loaded_mesh_count >= ctx->lib_mesh_capacity)
				ctx->lib_mesh_capacity = ctx->lib_mesh_capacity == 0 ? 8 : ctx->lib_mesh_capacity * 2;
			ctx->lib_mesh = realloc(ctx->lib_mesh, sizeof(t_mesh) * ctx->lib_mesh_capacity);
			if (!ctx->lib_mesh)
				fatal_error(ctx, errors(ERR_MALLOC), __FILE__, __LINE__);

			snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);
			printf("\033[1;33mLoading mesh:       %s\033[0m\n", file_path);

			t_mesh* mesh = &ctx->lib_mesh[ctx->loaded_mesh_count];
			strncpy(mesh->name, entry->d_name, sizeof(mesh->name) - 1);
			mesh->name[sizeof(mesh->name) - 1] = '\0';

			parse_obj(ctx, file_path, mesh);
			if (mesh->triangle_count > 0) {
				init_mesh_bvh(ctx, mesh);
				++ctx->loaded_mesh_count;
			}
		}
	}
	closedir(dir);
}

static inline bool is_obj_file(const char* file) {
	size_t len = strlen(file);
	if (len < 4)
		return false;
	return strcmp(file + len - 4, ".obj") == 0;
}

void add_mesh(t_context* ctx, const char* name, uint32_t mat_id, bool is_selected) {
	if (ctx->loaded_mesh_count == 0 || !ctx->lib_mesh) {
		printf("\033[1;31mError: Mesh library is empty.\033[0m\n");
		return;
	}

	t_mesh* mesh = NULL;
	for (uint32_t i = 0; i < ctx->loaded_mesh_count; ++i) {
		if (strcmp(ctx->lib_mesh[i].name, name) == 0) {
			mesh = &ctx->lib_mesh[i];
			break;
		}
	}

	if (!mesh) {
		printf("\033[1;31mError: Mesh '%s' not found in loaded assets.\033[0m\n", name);
		return;
	}
	if (mesh->triangle_count == 0 || mesh->bvh_root_idx == 0) {
		printf("\033[1;31mError: Mesh '%s' data is empty or invalid.\033[0m\n", name);
		return;
	}
	if (mat_id >= ctx->scene.assets.materials.total) {
		printf("\033[1;33mWarning: Material ID %u not found.\033[0m\n", mat_id);
		if (ctx->scene.assets.materials.total == 0) {
			printf("\033[1;31mFatal: No materials exist in the scene!\033[0m\n");
			return;
		}
		mat_id = 0;
	}

	t_object obj = (t_object){ //
		.type = OBJ_MESH,
		.material_id = mat_id,
		.transform.rot.w = 1.0f,
		.shape.mesh = *mesh
	};
	add_object(ctx, &obj, is_selected);
}

bool hit_mesh(const t_shape* shape, const t_ray* ray, t_hit* hit, uint32_t flags) {
	const t_mesh* mesh = &shape->mesh;
	return hit_bvh_mesh(mesh, ray, hit, flags);
}
