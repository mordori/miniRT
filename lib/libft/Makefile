# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/02 20:19:00 by myli-pen          #+#    #+#              #
#    Updated: 2025/10/14 00:20:42 by myli-pen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=libft.a

CONF		:=.config
BUILD_TYPE	?=RELEASE

WFLAGS		:=-Wall -Wextra -Werror -Wunreachable-code
CC			:=cc
OPTS		:=-O3 -march=native -funroll-loops -fno-plt
CFLAGS		?=$(WFLAGS) $(OPTS)
MAKEFLAGS	+= --no-print-directory

DIR_INC		:=inc/
DIR_SRC		:=src/
DIR_OBJ		:=obj/
DIR_DEP		:=dep/

INCS		:=-I $(DIR_INC)
SRCS		:=$(addprefix $(DIR_SRC), \
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
			ft_vector_utils.c ft_math.c ft_matrix.c ft_matrix_transforms.c \
			ft_vec4.c ft_vec3.c ft_vec3_2.c ft_matrix_utils.c \
			ft_vec2.c ft_vec4_2.c ft_vec2i.c ft_vec2i_2.c ft_math_2.c \
			ft_colors.c ft_vector_utils_2.c)
OBJS		:=$(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRCS))
DEPS		:=$(patsubst $(DIR_SRC)%.c, $(DIR_DEP)%.d, $(SRCS))

YELLOW		:=\033[1;33m
GREEN		:=\033[1;32m
RED			:=\033[1;31m
COLOR		:=\033[0m

all: config $(NAME)

config:
	@$(call check_config,$(BUILD_TYPE))

$(NAME): $(CONF) $(OBJS)
	@ar -rcs $@ $(OBJS)
	@$(call output)

$(DIR_OBJ)%.o: $(DIR_SRC)%.c $(CONF)
	@mkdir -p $(dir $@) $(patsubst $(DIR_OBJ)%, $(DIR_DEP)%, $(dir $@))
	@$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(INCS)

clean:
	@$(call rm_dir,$(DIR_OBJ))

fclean: clean
	@$(call rm_dir,$(DIR_DEP))
	@$(call rm_file,$(CONF))
	@$(call rm_file,$(NAME))

re: fclean all

.PHONY: all clean fclean re config
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
	@echo "$(YELLOW) [✔] $(NAME) built$(COLOR)"
endef
