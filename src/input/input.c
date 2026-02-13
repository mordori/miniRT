#include "input.h"
#include "camera.h"

void	process_input(t_context *ctx, t_renderer *r, bool *update)
{
	bool	dirty;


	// t_light	*light;
	// light = ((t_light **)ctx->scene.lights.items)[0];



	dirty = false;
	if (control_camera(ctx))
	{
		dirty = true;
		update_camera(ctx);
	}
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_P))
	{
		ctx->scene.skydome_uv_offset.u += 0.01f;
		if (ctx->scene.skydome_uv_offset.u > M_PI * 4.0f)
			ctx->scene.skydome_uv_offset.u = 0.0f;
		dirty = true;
	}
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_O))
	{
		ctx->scene.skydome_uv_offset.u -= 0.01f;
		if (ctx->scene.skydome_uv_offset.u < 0.0f)
			ctx->scene.skydome_uv_offset.u = M_PI * 4.0f;



		// light->pos =



		dirty = true;
	}

	if (dirty)
	{
		if (r->mode == RENDER_REFINE)
			atomic_store(&r->render_cancel, true);
		*update = true;
	}
}
