#include "editing.h"
#include "scene.h"
#include "utils.h"
#include "objects.h"

void	select_object(t_context *ctx)
{
	t_ray				ray;
	t_renderer			*r = &ctx->renderer;
	const t_viewport	*vp = &r->cam.viewport;
	t_vec2i				mouse;
	t_vec3				pixel_loc;
	t_vec3				dir;
	t_hit				hit;

	mlx_get_mouse_pos(ctx->mlx, &mouse.x, &mouse.y);
	pixel_loc = vec3_add(vp->pixel_00_loc, vec3_add(vec3_scale(vp->d_u, (float)mouse.x + 0.5f), vec3_scale(vp->d_v, (float)mouse.y + 0.5f)));
	dir = vec3_normalize(vec3_sub(pixel_loc, r->cam.transform.pos));
	ray = new_ray(r->cam.transform.pos, dir);
	hit = new_hit(0);
	if (
(int)hit_bvh_editing(ctx->scene.geo.bvh_root_idx, &ray, &hit, 0, ctx->scene.geo.bvh_nodes))
	{
		if (hit_object(ctx->editor.selected_obj, &ray, &hit) || hit.obj == ctx->scene.env.dir_light.obj)
			return ;
		if (ctx->editor.selected_obj)
			vector_try_add(ctx, &ctx->scene.geo.objs, ctx->editor.selected_obj);
		vector_remove(&ctx->scene.geo.objs, hit.obj);
		ctx->editor.selected_obj = hit.obj;
		if (!init_bvh(ctx))
		{
			pthread_mutex_unlock(&r->mutex);
			fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
		}
	}
	else if (!(int)hit_object(ctx->editor.selected_obj, &ray, &hit))
	{
		if (!ctx->editor.selected_obj)
			return ;
		vector_try_add(ctx, &ctx->scene.geo.objs, ctx->editor.selected_obj);
		ctx->editor.selected_obj = NULL;
		if (!init_bvh(ctx))
		{
			pthread_mutex_unlock(&r->mutex);
			fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
		}
	}
}
