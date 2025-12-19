#include "utils.h"

bool	resize_timer(t_context *ctx)
{
	uint32_t	time;

	time = time_now() - ctx->resize_time;
	return (time > 100);
}

/**
 * @brief Returns the current time in milliseconds.
 *
 * @return Time in milliseconds since Epoch.
 */
uint32_t	time_now(void)
{
	struct timeval	tv;
	uint32_t		time;

	gettimeofday(&tv, NULL);
	time = (uint32_t)tv.tv_sec * 1000 + (uint32_t)tv.tv_usec / 1000;
	return (time);
}
