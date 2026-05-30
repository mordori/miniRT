#include <stdlib.h>

#include "utils.h"

bool is_wsl(void) {
	return getenv("WSL_DISTRO_NAME") || getenv("WSL_INTEROP");
}
