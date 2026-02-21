#include "utils.h"

bool	resize_timer(const t_context *ctx)
{
	uint32_t	time;

	time = time_now() - ctx->resize_time;
	return (time > 250);
}

uint32_t	time_now(void)
{
	struct timeval	tv;
	uint32_t		time;

	gettimeofday(&tv, NULL);
	time = (uint32_t)tv.tv_sec * 1000 + (uint32_t)tv.tv_usec / 1000;
	return (time);
}

void	wait_until(uint32_t end)
{
	uint32_t	now;
	uint32_t	remaining;

	while (time_now() < end)
	{
		now = time_now();
		if (now >= end)
			break ;
		remaining = end - now;
		if (remaining > 2)
			usleep(1000);
		else
			usleep(500);
	}
}
