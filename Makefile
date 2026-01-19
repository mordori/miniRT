NAME		:=miniRT

CONF		:=.config
BUILD_TYPE	:=RELEASE

CC			:=cc
WFLAGS		:=-Wall -Wextra -Werror -Wunreachable-code
DEFS		:=
DFLAGS		:=-D DEBUG -g
OPTS		:=-O3 -march=native -funroll-loops -fno-plt -flto
CFLAGS		:=$(WFLAGS) $(DEFS) $(OPTS)
LDFLAGS		:=-ldl -lglfw -pthread -lm -flto
MAKEFLAGS	+= --no-print-directory

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
				light.c ambient.c directional.c point.c shadows.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_MAT), \
				material.c patterns.c textures.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_OBJECTS), \
				object.c cylinder.c plane.c sphere.c cone.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_PARSE), \
				parse.c parse_elements.c parse_objects.c parse_tools.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_RENDER), \
				renderer.c tracer.c post_processing.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_SCENE), \
				scene.c validator.c skydome.c bvh.c aabb.c bounds.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_UTILS), \
				errors.c files.c hooks.c strings.c vectors.c rays.c bounds.c noise.c mapping.c \
				textures.c renderer.c time.c bvh.c try_split.c ft_atof.c ft_strtod.c)
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
			ft_vector_utils.c ft_math.c ft_mat4.c ft_mat4_transforms.c \
			ft_vec4.c ft_vec3.c ft_vec3_2.c ft_mat4_utils.c ft_mat4_2.c \
			ft_vec2.c ft_vec4_2.c ft_vec2i.c ft_vec2i_2.c ft_math_2.c \
			ft_colors.c ft_vector_utils_2.c ft_vec3_3.c ft_mat2.c ft_mat3.c \
			ft_mat4_transforms_2.c ft_random.c)
OBJS		:=$(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRCS))
DEPS		:=$(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $(OBJS))

BLUE		:=\033[1;34m
YELLOW		:=\033[1;33m
GREEN		:=\033[1;32m
RED			:=\033[1;31m
COLOR		:=\033[0m

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
	@make -j4 -C $(DIR_MLX)build > /dev/null
	@echo "$(YELLOW) [✔] mlx42.a created$(COLOR)"

$(NAME): $(CONF) $(MLX42) $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(MLX42) $(LDFLAGS)
	@$(call output)

$(DIR_OBJ)%.o: $(DIR_SRC)%.c $(CONF) $(MLX42)
	@mkdir -p $(dir $@) $(patsubst $(DIR_OBJ)%, $(DIR_DEP)%, $(dir $@))
	@$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(INCS)
	@echo "$(GREEN) [+]$(COLOR) compiling $@"

clean:
	@$(call rm_dir,$(DIR_OBJ))

fclean: clean
	@$(call rm_dir,$(DIR_LIB))
	@$(call rm_dir,$(DIR_DEP))
	@$(call rm_file,$(CONF))
	@$(call rm_file,$(NAME))

re: fclean all

debug: BUILD_TYPE	:=DEBUG
debug: CFLAGS		:=$(WFLAGS) $(DEFS) $(DFLAGS) -O0
debug: LDFLAGS		:=-ldl -lglfw -pthread -lm
debug: all

.PHONY: all clean fclean re config debug
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
