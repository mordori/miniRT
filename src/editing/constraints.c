#include "editing.h"
#include "utils.h"

static inline void x_constraint(t_context* ctx);
static inline void y_constraint(t_context* ctx);
static inline void z_constraint(t_context* ctx);
static inline t_vec3 get_contraint_axis(t_context* ctx, t_vec3 global_axis, uint32_t col);

void set_axis_constraints(t_context* ctx, mlx_key_data_t keydata) {
	if (keydata.key != MLX_KEY_X && keydata.key != MLX_KEY_Y && keydata.key != MLX_KEY_Z)
		return;

	ctx->editor.constraint_axis = AXIS_DEFAULT;
	ctx->editor.constraints = 0;
	ctx->editor.axis_secondary = g_zero;

	switch (keydata.key) {
		case MLX_KEY_X: x_constraint(ctx); break;
		case MLX_KEY_Y: y_constraint(ctx); break;
		case MLX_KEY_Z: z_constraint(ctx); break;
		default: break;
	}
}

static inline t_vec3 get_contraint_axis(t_context* ctx, t_vec3 global_axis, uint32_t col) {
	if (ctx->editor.mode == EDIT_SCALE) {
		t_mat4* m = &ctx->editor.selected_obj->transform.object_to_world;
		t_vec3 axis = { //
			.x = m->m[0][col],
			.y = m->m[1][col],
			.z = m->m[2][col]
		};
		return vec3_normalize(axis);
	}
	return (global_axis);
}

static inline void x_constraint(t_context* ctx) {
	ctx->editor.constraint_axis = AXIS_X;
	ctx->editor.constraints = 1;
	ctx->editor.axis_primary = get_contraint_axis(ctx, g_right, 0);
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_SHIFT)) {
		ctx->editor.constraint_axis = AXIS_YZ;
		planar_basis(ctx, g_right);
		ctx->editor.constraints = 2;
	}
}

static inline void y_constraint(t_context* ctx) {
	ctx->editor.constraint_axis = AXIS_Y;
	ctx->editor.constraints = 1;
	ctx->editor.axis_primary = get_contraint_axis(ctx, g_up, 1);
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_SHIFT)) {
		ctx->editor.constraint_axis = AXIS_XZ;
		planar_basis(ctx, g_up);
		ctx->editor.constraints = 2;
	}
}

static inline void z_constraint(t_context* ctx) {
	ctx->editor.constraint_axis = AXIS_Z;
	ctx->editor.constraints = 1;
	ctx->editor.axis_primary = get_contraint_axis(ctx, g_forward, 2);
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_SHIFT)) {
		ctx->editor.constraint_axis = AXIS_XY;
		planar_basis(ctx, g_forward);
		ctx->editor.constraints = 2;
	}
}
