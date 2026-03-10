#include "editing.h"
#include "scene.h"
#include "utils.h"

void	select_object(t_context *ctx)
{
	t_ray				ray;
	const t_renderer	*r = &ctx->renderer;
	const t_viewport	*vp = &r->cam.viewport;
	t_vec2i				mouse;
	t_vec3				orig;
	t_vec3				dir;
	t_hit				hit;

	mlx_get_mouse_pos(ctx->mlx, &mouse.x, &mouse.y);
	orig = vec3_add(vec3_scale(vp->d_u, (float)mouse.x + 0.5f), vec3_scale(vp->d_v, (float)mouse.y + 0.5f));
	dir = vec3_normalize(vec3_add(orig, vec3(0.0f, 0.0f, 1.0f)));
	ray = new_ray(orig, dir);
	hit = new_hit(0);
	if (
(int)hit_bvh(ctx->scene.geo.bvh_root_idx, &ray, &hit, 0, ctx->scene.geo.bvh_nodes))
	{
		vector_try_add(ctx, &ctx->scene.geo.objs, ctx->selected_obj);
		// TODO: remove specific item from vector
		vector_del(&ctx->scene.geo.objs, (void *)hit.obj - *ctx->scene.geo.objs.items);
		ctx->selected_obj = hit.obj;
		// TODO: outline?
		printf("SELECTED\n");
	}
}
