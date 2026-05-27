#include "utils.h"

bool resize_timer(const t_context* ctx) {
	uint32_t time = time_now() - ctx->resize_time;
	return time > 250;
}

uint32_t time_now(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint32_t time = (uint32_t)tv.tv_sec * 1000 + (uint32_t)tv.tv_usec / 1000;
	return time;
}

uint32_t engine_time(void) {
	static struct timeval start;
	if (start.tv_sec == 0)
		gettimeofday(&start, NULL);

	struct timeval now;
	gettimeofday(&now, NULL);
	uint32_t time = (now.tv_sec - start.tv_sec) * 1000;
	time += now.tv_usec / 1000;
	time -= start.tv_usec / 1000;
	return time;
}

void wait_until(uint32_t end) {
	while (time_now() < end) {
		uint32_t now = time_now();
		if (now >= end)
			break;

		uint32_t remaining = end - now;
		if (remaining > 2)
			usleep(1000);
		else
			usleep(500);
	}
}

char* timestamp(char* buf) {
	time_t now = time(NULL);
	strftime(buf, 32, "%Y-%m-%d_%H-%M-%S", localtime(&now));
	return buf;
}
