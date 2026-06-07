#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libft_io.h"
#include "libft_str.h"
#include "utils.h"

ssize_t try_write(t_context* ctx, int fd, const char* src) {
	size_t len = ft_strlen(src);
	if (len > SIZE_MAX)
		fatal_error(ctx, errors(ERR_SIZE_MAX), __FILE__, __LINE__);

	ssize_t bytes = write(fd, src, len);
	if (bytes != (ssize_t)len) {
		close(fd);
		fatal_error(ctx, errors(ERR_WRITE), __FILE__, __LINE__);
	}
	return bytes;
}

ssize_t try_write_endl(t_context* ctx, int fd, const char* src) {
	ssize_t bytes = try_write(ctx, fd, src);
	bytes += try_write(ctx, fd, "\n");
	return bytes;
}

int try_open(t_context* ctx, const char* file, int o_flag, int p_flag) {
	int fd = open(file, o_flag, p_flag);
	if (fd == ERROR)
		fatal_error(ctx, errors(ERR_OPEN), __FILE__, __LINE__);
	return fd;
}

ssize_t try_read(t_context* ctx, int fd, char* buf, size_t n_bytes) {
	ssize_t bytes = read(fd, buf, n_bytes);
	if (bytes == ERROR) {
		close(fd);
		fatal_error(ctx, errors(ERR_READ), __FILE__, __LINE__);
	}
	return bytes;
}

int try_gnl(t_context* ctx, int fd, char** line) {
	int status = get_next_line(fd, line);
	if (status == GNL_ERROR)
		fatal_error(ctx, errors(ERR_GNL), __FILE__, __LINE__);
	return status;
}

void make_dir(t_context* ctx, const char* path) {
	if (mkdir(path, 0777) == ERROR) {
		if (errno != EEXIST)
			fatal_error(ctx, errors(ERR_DIR), __FILE__, __LINE__);
	}
}

void open_image_viewer(const char* filepath) {
	char command[512];

#if defined(_WIN32)
	snprintf(command, sizeof(command), "cmd.exe /c start \"\" \"%s\"", filepath);
#elif defined(__APPLE__)
	snprintf(command, sizeof(command), "open \"%s\"", filepath);
#elif defined(__linux__)
	if (is_wsl()) {
		char abs_path[PATH_MAX];
		if (realpath(filepath, abs_path) != NULL) {
			snprintf(
				command, sizeof(command), "powershell.exe -WindowStyle Hidden -Command \"Start-Process '$(wslpath -w '%s')'\"", abs_path);
		} else {
			return;
		}
	} else {
		snprintf(command, sizeof(command), "xdg-open \"%s\" &", filepath);
	}
#else
	return;
#endif

	int result = system(command);
	(void)result;
}
