# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/30 19:17:42 by myli-pen          #+#    #+#              #
#    Updated: 2025/12/07 06:19:39 by myli-pen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=miniRT

CONF		:=.config
BUILD_TYPE	:=RELEASE

CC			:=clang
WFLAGS		:=-Wall -Wextra -Werror -Wunreachable-code
DEFS		:=
DFLAGS		:=-D DEBUG -g
OPTS		:=-O3 -march=native -funroll-loops -fno-plt -ffast-math
CFLAGS		:=$(WFLAGS) $(DEFS) $(OPTS)
LDFLAGS		:=-ldl -lglfw -pthread -lm -flto
MAKEFLAGS	+= --no-print-directory

DIR_INC		:=inc/
DIR_SRC		:=src/
DIR_LIB		:=lib/
DIR_OBJ		:=obj/
DIR_DEP		:=dep/
DIR_UTILS	:=utils/

DIR_LIBFT	:=$(DIR_LIB)libft/
DIR_MLX		:=$(DIR_LIB)MLX42/

DIR_CAM		:=camera/
DIR_EDIT	:=editing/
DIR_INPUT	:=input/
DIR_LIGHTS	:=lights/
DIR_MAT		:=materials/
DIR_OBJECTS	:=objects/
DIR_RENDER	:=rendering/
DIR_SCENE	:=scene/
DIR_UTILS	:=utils/

LIBFT		:=$(DIR_LIBFT)libft.a

URL_MLX		:=https://github.com/codam-coding-college/MLX42.git
MLX42		:=$(DIR_MLX)build/libmlx42.a

INCS		:=$(addprefix -I, \
				$(DIR_MLX)include/MLX42 \
				$(DIR_LIBFT)$(DIR_INC) \
				$(DIR_INC))

SRCS		:=$(addprefix $(DIR_SRC), \
				main.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_CAM), \
				camera.c controls.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_EDIT), \
				)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_INPUT), \
				mouse.c keys.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_LIGHTS), \
				light.c ambient.c directional.c point.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_MAT), \
				material.c patterns.c textures.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_OBJECTS), \
				object.c cylinder.c plane.c sphere.c cone.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_RENDER), \
				renderer.c shadows.c tracer.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_SCENE), \
				scene.c validator.c skydome.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_UTILS), \
				errors.c files.c hooks.c strings.c vectors.c rays.c)
OBJS		:=$(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRCS))
DEPS		:=$(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $(OBJS))

BLUE		:=\033[1;34m
YELLOW		:=\033[1;33m
GREEN		:=\033[1;32m
RED			:=\033[1;31m
COLOR		:=\033[0m

all: config $(LIBFT) $(MLX42) $(NAME)

config:
	@$(call check_config,$(BUILD_TYPE))

$(LIBFT): $(CONF)
	@if [ ! -e "$(LIBFT)" ] || [ "$$(head -n 1 $(DIR_LIBFT)$(CONF))" != "$(BUILD_TYPE)" ]; then \
		echo "$(GREEN) [+]$(COLOR) compiling libft.a"; \
		make -C $(DIR_LIBFT) BUILD_TYPE="$(BUILD_TYPE)" CFLAGS="$(CFLAGS)"; \
	fi

$(MLX42):
	@if [ ! -d "$(DIR_MLX)" ]; then \
		echo "$(BLUE) [~]$(COLOR) cloning to lib/ $(BLUE)$(URL_MLX)$(COLOR)"; \
		git clone --quiet $(URL_MLX) $(DIR_MLX); \
	fi
	@echo "$(GREEN) [+]$(COLOR) compiling mlx42.a"
	@cmake $(DIR_MLX) -B $(DIR_MLX)build > /dev/null
	@make -j4 -C $(DIR_MLX)build > /dev/null
	@echo "$(YELLOW) [✔] mlx42.a created$(COLOR)"

$(NAME): $(CONF) $(LIBFT) $(MLX42) $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT) $(MLX42) $(LDFLAGS)
	@$(call output)

$(DIR_OBJ)%.o: $(DIR_SRC)%.c $(CONF) $(LIBFT) $(MLX42)
	@mkdir -p $(dir $@) $(patsubst $(DIR_OBJ)%, $(DIR_DEP)%, $(dir $@))
	@$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(INCS)
	@echo "$(GREEN) [+]$(COLOR) compiling $@"

clean:
	@make -C $(DIR_LIBFT) clean
	@$(call rm_dir,$(DIR_OBJ))

fclean: clean
	@make -C $(DIR_LIBFT) fclean
	@$(call rm_dir,$(DIR_MLX))
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
	echo "$(GREEN) [/] usage: $(YELLOW)./$(NAME) 'assets/scenes/scene_name.rt$(COLOR)'"; \
	if [ "$(BUILD_TYPE)" = "DEBUG" ]; then \
		echo "$(YELLOW) [DEBUG]$(COLOR)"; \
	fi;
endef
