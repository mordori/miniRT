# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/30 19:17:42 by myli-pen          #+#    #+#              #
#    Updated: 2025/11/30 23:21:49 by myli-pen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=miniRT

MEMORY		?=16777216
CONF		:=.config
BUILD_TYPE	:=RELEASE

WFLAGS		:=-Wall -Wextra -Werror -Wunreachable-code
DEFS		:=-D MEMORY=$(MEMORY)
DFLAGS		:=-D DEBUG -g
OPTS		:=-O3 -march=native -funroll-loops -fno-plt -ffast-math
CC			:=cc
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
DIR_INPUT	:=input/
DIR_OBJECTS	:=objects/
DIR_PARS	:=parsing/
DIR_RENDER	:=rendering/
DIR_UTILS	:=utils/

LIBFT		:=$(DIR_LIBFT)libft.a

URL_MLX		:=https://github.com/codam-coding-college/MLX42.git
MLX42		:=$(DIR_MLX)build/libmlx42.a

INCS		:=$(addprefix -I, \
				$(DIR_MLX)include/MLX42 \
				$(DIR_LIBFT)$(DIR_INC) \
				$(DIR_INC) \
				)

SRCS		:=$(addprefix $(DIR_SRC), \
				main.c)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_CAM), \
				)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_INPUT), \
				)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_OBJECTS), \
				)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_PARS), \
				)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_RENDER), \
				)
SRCS		+=$(addprefix $(DIR_SRC)$(DIR_UTILS), \
				)
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
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBFT) $(MLX42)
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
		echo "" >> $(CONF); \
	fi
	@if [ "$$(head -n 1 $(CONF))" != "$(1)" ]; then \
		sed -i '1c\$(1)' "$(CONF)"; \
	fi
	@if [ "$$(head -n 2 $(CONF) | tail -n 1)" != "$(MEMORY)" ]; then \
		sed -i '2c\$(MEMORY)' "$(CONF)"; \
	fi
endef

define output
	@if [ $$(($(MEMORY))) -lt 1024 ]; then \
		echo "$(YELLOW) [✔] $(NAME) built with invalid amount of memory (1 KiB is minimum)$(COLOR)"; \
		echo "$(RED) [/] the program will produce an error when run$(COLOR)"; \
	elif [ $$(($(MEMORY) & ($(MEMORY) - 1))) -ne 0 ]; then \
		echo "$(YELLOW) [✔] $(NAME) built with non power of two amount of memory$(COLOR)"; \
		echo "$(RED) [/] the program will produce an error when run$(COLOR)"; \
	elif [ $$(($(MEMORY)/1024/1024)) -lt 1 ]; then \
		echo "$(YELLOW) [✔] $(NAME) built with $$(echo "scale=1; $(MEMORY)/1024" | bc) KiB memory$(COLOR)"; \
	elif [ $$(($(MEMORY)/1024/1024)) -lt 1000 ]; then \
		echo "$(YELLOW) [✔] $(NAME) built with $$(echo "scale=1; $(MEMORY)/1024/1024" | bc) MiB memory$(COLOR)"; \
	else \
		echo "$(YELLOW) [✔] $(NAME) built with $$(echo "scale=1; $(MEMORY)/1024/1024/1024" | bc) GiB memory$(COLOR)"; \
	fi
	@if [ $$(($(MEMORY))) -ge 1024 ] && [ $$(($(MEMORY) & ($(MEMORY) - 1))) -eq 0 ]; then \
		echo "$(GREEN) [/] usage: ./$(NAME)$(COLOR)"; \
		if [ "$(BUILD_TYPE)" = "DEBUG" ]; then \
			echo "$(YELLOW) [DEBUG]$(COLOR)"; \
		fi; \
	fi
endef
