#include "utils.h"

void set_text_color(mlx_image_t* img, uint32_t color) {
	uint32_t pixels = img->width * img->height * 4u;
	uint32_t i = 0;
	while (i < pixels) {
		if (img->pixels[i + 3] > 0) {
			img->pixels[i + 0] = (color >> 24) & 0xFF;
			img->pixels[i + 1] = (color >> 16) & 0xFF;
			img->pixels[i + 2] = (color >> 8) & 0xFF;
		}
		i += 4;
	}
}

t_ui ui_val(int32_t* last, int32_t current) {
	return (t_ui){ //
		.last = last,
		.current = current
	};
}

t_ui ui_valf(float* lastf, float currentf) {
	return (t_ui){ //
		.lastf = lastf,
		.currentf = currentf
	};
}
