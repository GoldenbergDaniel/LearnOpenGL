NAME = Renderer
LDFLAGS = -framework OpenGL -lsdl2 -L. -lglad
CFLAGS = -Ilib/ -std=c++17 -O0 -Wall \
	-Wno-deprecated-declarations \
	-Wno-missing-braces \
	-Wno-unused-variable
CC = c++

all: compile run

compile:
	$(CC) $(CFLAGS) $(LDFLAGS) src/main.cpp src/gfx.cpp -o $(NAME) 

run:
	./$(NAME)

test:
	cd debug; \
	$(CC) $(LDFLAGS) ../lib/glad/glad.cpp ../src/main.cpp -g

compile_glad:
	$(CC) $(CFLAGS) -dynamiclib -o libglad.dylib lib/glad/glad.cpp
