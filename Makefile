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
SRC = src/main.c \
	src/matrices.c \
	src/matrix_transformations.c \
	src/parse_obj.c \
	src/load_texture.c \
	src/load_shader.c \
	src/events.c \
	src/init_gl.c \
	src/background.c \
	src/exit.c \
	glad/src/glad.c
OBJ = $(SRC:.c=.o)
UNAME = $(shell uname -s)
CFLAGS = -I ./glfw/include -I ./glad/include -Wall -Wextra -Werror
ifneq (, $(findstring MINGW, $(UNAME)))
	LDFLAGS = -L ./glfw/lib-mingw -lglfw3 -lgdi32
else ifeq ($(UNAME), Darwin)
	# https://www.glfw.org/docs/3.0/build.html#build_link_osx
	LDFLAGS = -L ./glfw/lib-macos -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
endif
.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -rf $(OBJ)

fclean:
	rm -rf $(OBJ)
	rm -rf $(NAME)

re: fclean all
