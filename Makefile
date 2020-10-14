# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/21 15:08:28 by gbiebuyc          #+#    #+#              #
#    Updated: 2020/01/21 17:03:26 by gbiebuyc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = scop
SRC = main.c \
	matrices.c \
	matrix_transformations.c \
	parse_obj.c \
	parse_obj_2.c \
	load_texture.c \
	load_shader.c \
	events.c \
	init_gl.c \
	background.c \
	dynarray.c \
	utils.c \
	model.c \
	cubemap.c \
	draw.c \
	exit.c
OBJ = $(addprefix ./obj/, $(SRC:.c=.o))
UNAME = $(shell uname -s)
CFLAGS = -I ./glfw/include -I ./glad/include -Wall -Wextra -Werror
ifneq (, $(findstring MINGW, $(UNAME)))
	NAME := $(NAME).exe
	LDFLAGS = -L ./glfw/lib-mingw -lglfw3 -lgdi32
else ifeq ($(UNAME), Darwin)
	# https://www.glfw.org/docs/3.0/build.html#build_link_osx
	LDFLAGS = -L ./glfw/lib-macos -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
else ifeq ($(UNAME), Linux)
	LDFLAGS = -L ./glfw/lib-linux -lglfw3 -lm -ldl -lX11 -lpthread
endif
.PHONY: all clean fclean re

all: ./obj $(NAME)

./obj:
	mkdir -p ./obj

./obj/%.o: ./src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJ) ./obj/glad.o
	$(CC) -o $@ $^ $(LDFLAGS)

./obj/glad.o: glad/src/glad.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf ./obj

fclean: clean
	rm -rf $(NAME)

re: fclean all
