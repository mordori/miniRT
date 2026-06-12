NAME		:=miniPT

CC			:=clang
WFLAGS		:=-Wall -Wextra -Werror -Wpedantic -Wunreachable-code -Wshadow -Wnull-dereference -Wfloat-equal -Wcast-align -Wformat=2 -Wundef
DEFS		:=-D_GNU_SOURCE -D_DARWIN_C_SOURCE
DFLAGS		:=-D DEBUG -g
DEBUG		:=-O0 -g
# -fsanitize=address,undefined,alignment,float-cast-overflow,float-divide-by-zero -fno-omit-frame-pointer

CXX			:=clang++
STD			:=-std=c++20
WFLAGS_CPP	:=-Wall -Wextra -Werror -Wpedantic -Wunreachable-code -Wshadow -Wnull-dereference -Wfloat-equal -Wformat=2 -Wundef -Wno-gnu-anonymous-struct -Wno-c99-extensions -Wno-c11-extensions

UNAME_S		:=$(shell uname -s)
UNAME_M		:=$(shell uname -m)
OPTS		:=-O3 -ffast-math -funroll-loops -flto -DNDEBUG
LDFLAGS		:=-lglfw -pthread -lm -flto
ifeq ($(UNAME_S),Darwin)
	LDFLAGS		+= -L"/opt/homebrew/lib" -L"/usr/local/lib" -L"/opt/local/lib" -framework Cocoa -framework OpenGL -framework IOKit
	ifeq ($(UNAME_M),arm64)
		OPTS	+= -mcpu=native
	else
		OPTS	+= -march=native
	endif
else
	LDFLAGS		+= -ldl
	OPTS		+= -march=haswell -fno-plt
endif

CFLAGS		:=$(WFLAGS) $(DEFS) $(OPTS)
CXXFLAGS	:=$(STD) $(WFLAGS_CPP) $(DEFS) $(OPTS)
ifeq ($(MAKELEVEL),0)
	MAKEFLAGS	+= --no-print-directory -j$(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
endif

DIR_NAME	:=$(notdir $(CURDIR))
DIR_INC		:=inc/
DIR_SRC		:=src/
DIR_LIB		:=lib/
DIR_OBJ		:=obj/
DIR_DEP		:=dep/
DIR_UTILS	:=utils/

DIR_MLX		:=$(DIR_LIB)MLX42/
DIR_OIDN	:=$(DIR_LIB)IntelOID/
DIR_LIBFT	:=libft/

DIR_CAM		:=camera/
DIR_EDIT	:=editing/
DIR_INPUT	:=input/
DIR_LIGHTS	:=lights/
DIR_MAT		:=materials/
DIR_OBJECTS	:=objects/
DIR_PARSE	:=parsing/
DIR_RENDER	:=rendering/
DIR_SCENE	:=scene/
DIR_UI		:=ui/

URL_MLX		:=https://github.com/codam-coding-college/MLX42.git
MLX42		:=$(DIR_MLX)build/libmlx42.a

INCS		:=$(addprefix -I, \
				$(DIR_MLX)include/MLX42 \
				$(DIR_OIDN)include \
				$(DIR_INC) \
				$(DIR_INC)$(DIR_LIBFT) \
				$(DIR_SRC)$(DIR_UI))
ifeq ($(UNAME_S),Darwin)
	INCS	+= -I"/opt/homebrew/include" -I"/usr/local/include" -I"/opt/local/include"
endif

LDFLAGS		+= -L$(DIR_OIDN)lib -lOpenImageDenoise -Wl,-rpath,$(CURDIR)/$(DIR_OIDN)lib

SRCS		:=$(addprefix $(DIR_SRC), \
				main.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_CAM), \
				camera.c controls.c controls_router.c actions.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_EDIT), \
				select.c edit.c constraints.c translate.c rotate.c scale.c actions.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_INPUT), \
				input_router.c mouse.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_LIGHTS), \
				point.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_MAT), \
				material.c patterns.c textures.c perlin_noise.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_OBJECTS), \
				object.c sphere.c quad.c triangle.c mesh.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_PARSE), \
				parse_mesh.c \
				parse.c parse_elements.c parse_light.c parse_objects.c try_split.c \
				parse_tools.c parse_materials.c parse_textures.c parse_utils.c \
				parse_pass.c parse_patterns.c parse_validate.c ft_strtod.c ft_strtof.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_RENDER), \
				editing/tracer.c editing/lighting.c \
				renderer.c tracer.c post_processing.c environment.c window.c \
				frame.c bsdf.c brdf.c lighting.c sampling.c screenshot.c projection.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_SCENE), \
				scene.c bvh.c bounds.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_UTILS), \
				errors.c files.c ray.c memory.c random.c hit.c instructions.c time.c system.c)
SRCS		+= $(addprefix $(DIR_SRC)$(DIR_UTILS)$(DIR_LIBFT), \
				ft_atoi.c ft_atof.c ft_isalpha.c ft_itoa.c ft_memmove.c ft_putnbr_fd.c \
				ft_bzero.c ft_isascii.c ft_memchr.c ft_memset.c ft_toupper.c \
				ft_calloc.c ft_isdigit.c ft_memcmp.c ft_putchar_fd.c \
				ft_isalnum.c ft_isprint.c ft_memcpy.c ft_putendl_fd.c \
				ft_strdup.c ft_strlcpy.c ft_strnstr.c ft_tolower.c \
				ft_putstr_fd.c ft_striteri.c ft_strlen.c ft_strrchr.c \
				ft_split.c ft_strjoin.c ft_strmapi.c ft_strtrim.c \
				ft_strchr.c ft_strlcat.c ft_strncmp.c ft_substr.c \
				ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c \
				ft_lstclear_bonus.c ft_lstdelone_bonus.c \
				ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c \
				ft_lstnew_bonus.c ft_lstsize_bonus.c ft_printf.c ft_uitoa.c \
				ft_countdigits.c ft_strchrdup.c ft_get_next_line.c ft_vector.c \
				ft_vector_utils.c ft_vector_utils_2.c)
OBJS		:=$(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRCS))
DEPS		:=$(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $(OBJS))

SRCS_CPP	:=$(addprefix $(DIR_SRC)$(DIR_UI), \
				ui.cpp imgui.cpp imgui_draw.cpp imgui_impl_glfw.cpp imgui_impl_opengl3.cpp \
				imgui_tables.cpp imgui_widgets.cpp)
OBJS_CPP	:=$(patsubst $(DIR_SRC)%.cpp, $(DIR_OBJ)%.o, $(SRCS_CPP))
DEPS_CPP	:=$(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $(OBJS_CPP))

BLUE		:=\033[1;34m
YELLOW		:=\033[1;33m
GREEN		:=\033[1;32m
RED			:=\033[1;31m
COLOR		:=\033[0m

all: $(MLX42) $(NAME)

compdb:
	@if command -v bear >/dev/null 2>&1; then \
		bear --append -- $(MAKE) all; \
	elif command -v compiledb >/dev/null 2>&1; then \
		compiledb $(MAKE) all; \
	else \
		$(MAKE) all; \
	fi

$(MLX42):
	@if [ ! -d "$(DIR_MLX)" ]; then \
		echo "$(BLUE) [~]$(COLOR) cloning to lib/ $(BLUE)$(URL_MLX)$(COLOR)"; \
		git clone --quiet $(URL_MLX) $(DIR_MLX); \
	fi
	@echo "$(GREEN) [+]$(COLOR) compiling mlx42.a"
	@cmake $(DIR_MLX) -B $(DIR_MLX)build > /dev/null
	@+make -C $(DIR_MLX)build > /dev/null
	@echo "$(YELLOW) [✔] mlx42.a created$(COLOR)"

$(NAME): $(MLX42) $(OBJS) $(OBJS_CPP)
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(OBJS_CPP) $(MLX42) $(LDFLAGS)
	@$(call output)

$(DIR_OBJ)%.o: $(DIR_SRC)%.c $(MLX42)
	@$(call compile_objs)

$(DIR_OBJ)%.o: $(DIR_SRC)%.cpp
	@$(call compile_cpp_objs)

clean:
	@$(call rm_dir,$(DIR_OBJ))
	@$(call rm_dir,.cache/)

fclean: clean
	@$(call rm_dir,$(DIR_DEP))
	@$(call rm_file,$(NAME))
	@$(call rm_file,compile_commands.json)

re:
	@$(MAKE) fclean
	@$(MAKE) all

debug:
	@$(MAKE) fclean
	@$(MAKE) all OPTS="$(DEBUG)" BUILD_TYPE="DEBUG"

debugdb:
	@$(MAKE) fclean
	@$(MAKE) compdb OPTS="$(DEBUG)" BUILD_TYPE="DEBUG"

.PHONY: all clean fclean re debug compdb debugdb
.SECONDARY: $(OBJS) $(OBJS_CPP) $(DEPS) $(DEPS_CPP)

-include $(DEPS) $(DEPS_CPP)

define compile_objs
	@mkdir -p $(dir $@) $(patsubst $(DIR_OBJ)%, $(DIR_DEP)%, $(dir $@))
	@$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(INCS)
	@echo "$(GREEN) [+]$(COLOR) compiling $@"
endef

define compile_cpp_objs
	@mkdir -p $(dir $@) $(patsubst $(DIR_OBJ)%, $(DIR_DEP)%, $(dir $@))
	@$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(INCS)
	@echo "$(GREEN) [+]$(COLOR) compiling $@"
endef

define rm_dir
	@if [ -d "$(1)" ]; then \
		rm -rf $(1); \
		echo "$(RED) [-]$(COLOR) removed $(1)"; \
	fi
endef

define rm_file
	@if [ -e "$(1)" ]; then \
		rm -f $(1); \
		echo "$(RED) [-]$(COLOR) removed $(1)"; \
	fi
endef

define output
	echo "$(YELLOW) [✔] $(NAME) created$(COLOR)"
	echo "$(GREEN) [/] usage: $(YELLOW)./$(NAME)$(COLOR)"; \
	if [ "$(BUILD_TYPE)" = "DEBUG" ]; then \
		echo "$(YELLOW) [DEBUG]$(COLOR)"; \
	fi;
endef
