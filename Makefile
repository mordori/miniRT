NAME		:=miniRT

CONF		:=.config
BUILD_TYPE	:=RELEASE

CC			:=gcc
WFLAGS		:=-Wall -Wextra -Werror -Wunreachable-code -Wshadow \
			 -Wnull-dereference -Wfloat-equal -Wcast-align -Wformat=2 -Wswitch-enum -Wundef
DEFS		:=
DFLAGS		:=-D DEBUG -g
SANFLAGS	:=-fsanitize=address,undefined,alignment -fno-omit-frame-pointer
OPTS		:=-Ofast -march=haswell -funroll-loops -fno-plt -flto
CFLAGS		:=$(WFLAGS) $(DEFS) $(OPTS)
LDFLAGS		:=-ldl -lglfw -pthread -lm -flto
MAKEFLAGS	+= --no-print-directory -j$(shell nproc 2>/dev/null || echo 4)

DIR_INC		:=inc/
DIR_SRC		:=src/
DIR_LIB		:=lib/
DIR_OBJ		:=obj/
DIR_DEP		:=dep/
DIR_UTILS	:=utils/

DIR_MLX		:=$(DIR_LIB)MLX42/

DIR_CAM		:=camera/
DIR_EDIT	:=editing/
DIR_INPUT	:=input/
DIR_LIGHTS	:=lights/
DIR_MAT		:=materials/
DIR_OBJECTS	:=objects/
DIR_PARSE	:=parsing/
DIR_RENDER	:=rendering/
DIR_SCENE	:=scene/
DIR_UTILS	:=utils/
DIR_LIBFT	:=libft/
DIR_MATH	:=lib_math/

URL_MLX		:=https://github.com/codam-coding-college/MLX42.git
MLX42		:=$(DIR_MLX)build/libmlx42.a

INCS		:=$(addprefix -I, \
				$(DIR_MLX)include/MLX42 \
				$(DIR_INC) \
				$(DIR_INC)$(DIR_LIBFT))

SRCS		:=$(addprefix $(DIR_SRC), \
				main.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_CAM), \
				camera.c controls.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_EDIT), \
				)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_INPUT), \
				input.c mouse.c keys.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_LIGHTS), \
				lighting.c ambient.c point.c shadows.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_MAT), \
				material.c patterns.c textures.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_OBJECTS), \
				object.c cylinder.c plane.c sphere.c cone.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_PARSE), \
				parse.c parse_elements.c parse_objects.c parse_tools.c parse_materials.c parse_textures.c parse_utils.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_RENDER), \
				renderer.c tracer.c post_processing.c skydome.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_SCENE), \
				scene.c validator.c bvh.c aabb.c bounds.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_UTILS), \
				errors.c files.c hooks.c strings.c vectors.c ray.c bounds.c noise.c mapping.c range.c alloc.c \
				textures.c renderer.c time.c bvh.c try_split.c ft_atof.c ft_strtod.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_UTILS)$(DIR_MATH), \
				math.c mat4.c mat4_transforms.c \
				vec4.c vec3.c vec3_2.c mat4_utils.c mat4_inverse.c \
				vec2.c vec4_2.c vec2i.c vec2i_2.c math_2.c math_3.c \
				vec3_3.c color.c v4sf.c v4si.c coords.c sampling.c \
				mat4_transforms_2.c random.c vec3_4.c vec3_5.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_UTILS)$(DIR_LIBFT), \
				ft_atoi.c ft_isalpha.c ft_itoa.c ft_memmove.c ft_putnbr_fd.c \
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

BLUE		:=\033[1;34m
YELLOW		:=\033[1;33m
GREEN		:=\033[1;32m
RED			:=\033[1;31m
COLOR		:=\033[0m

# Removed config dependencies to use parallel compilation
# For debug commands to work, add $(CONF) back to NAME and %.o and compile serially
# TODO: remove config completely for shipping
all: config $(MLX42) $(NAME)

config:
	@$(call check_config,$(BUILD_TYPE))

$(MLX42):
	@if [ ! -d "$(DIR_MLX)" ]; then \
		echo "$(BLUE) [~]$(COLOR) cloning to lib/ $(BLUE)$(URL_MLX)$(COLOR)"; \
		git clone --quiet $(URL_MLX) $(DIR_MLX); \
	fi
	@echo "$(GREEN) [+]$(COLOR) compiling mlx42.a"
	@cmake $(DIR_MLX) -B $(DIR_MLX)build > /dev/null
	@+make -C $(DIR_MLX)build > /dev/null
	@echo "$(YELLOW) [✔] mlx42.a created$(COLOR)"

$(NAME): $(MLX42) $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(MLX42) $(LDFLAGS)
	@$(call output)

$(DIR_OBJ)%.o: $(DIR_SRC)%.c $(MLX42)
	@mkdir -p $(dir $@) $(patsubst $(DIR_OBJ)%, $(DIR_DEP)%, $(dir $@))
	@$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(INCS)
	@echo "$(GREEN) [+]$(COLOR) compiling $@"

clean:
	@$(call rm_dir,$(DIR_OBJ))

fclean: clean
# 	@$(call rm_dir,$(DIR_LIB))
	@$(call rm_dir,$(DIR_DEP))
	@$(call rm_file,$(CONF))
	@$(call rm_file,$(NAME))
	@$(call rm_file,$(NAME)_sanitize)

re:
	@$(MAKE) fclean 2> /dev/null
	@$(MAKE) all

debug: BUILD_TYPE	:=DEBUG
debug: CFLAGS		:=$(WFLAGS) $(DEFS) $(DFLAGS) -O0
debug: LDFLAGS		:=-ldl -lglfw -pthread -lm
debug: all

sanitize: BUILD_TYPE	:=SANITIZE
sanitize: CFLAGS		:=$(WFLAGS) $(DEFS) $(DFLAGS) $(SANFLAGS) -O1
sanitize: LDFLAGS		:=-ldl -lglfw -pthread -lm $(SANFLAGS)
sanitize: NAME			:=$(NAME)_sanitize
sanitize: all

.PHONY: all clean fclean re config debug sanitize
.SECONDARY: $(OBJS) $(DEPS) $(CONF)

-include $(DEPS)

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

define check_config
	@if [ ! -e "$(CONF)" ]; then \
		touch "$(CONF)"; \
		echo "" >> $(CONF); \
	fi
	@if [ "$$(head -n 1 $(CONF))" != "$(1)" ]; then \
		sed -i '1c\$(1)' "$(CONF)"; \
	fi
endef

define output
	echo "$(YELLOW) [✔] $(NAME) created$(COLOR)"
	echo "$(GREEN) [/] usage: $(YELLOW)./$(NAME) 'assets/scenes/scene_name.rt$(COLOR)'"; \
	if [ "$(BUILD_TYPE)" = "DEBUG" ]; then \
		echo "$(YELLOW) [DEBUG]$(COLOR)"; \
	fi;
endef
