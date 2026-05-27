#include "lib_math.h"
#include "utils.h"

void random_uv(const t_context* ctx, t_path* path, t_pixel* pixel, t_bn_channel c) {
	uint32_t dim_u = c + (path->bounce * ctx->bn_stride);
	uint32_t dim_v = c + 1 + (path->bounce * ctx->bn_stride);

	if (path->bounce == 0) {
		path->uv = (t_vec2){ //
			.u = blue_noise(&ctx->tex_bn, pixel, dim_u),
			.v = blue_noise(&ctx->tex_bn, pixel, dim_v)
		};
	} else {
		t_vec2 offset = { //
			.u = static_blue_noise(&ctx->tex_bn, pixel, dim_u),
			.v = static_blue_noise(&ctx->tex_bn, pixel, dim_v)
		};
		path->uv = r2_sequence(pixel->frame + (path->bounce * FP_PRIME), offset);
	}
}
