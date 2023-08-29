NAME = Renderer
LDFLAGS = -Ilib/ -framework OpenGL -lsdl2 -L. -lglad
CFLAGS = -std=c++17 -O1 -Wall -fno-exceptions \
	-Wno-deprecated-declarations \
	-Wno-missing-braces \
	-Wno-unused-variable
CC = c++

all: compile run

compile:
	@echo "Compiling..."
	@$(CC) $(CFLAGS) $(LDFLAGS) src/main.cpp src/render.cpp -o $(NAME) 

compile_t:
	@echo "Compiling..."
	@time $(CC) $(CFLAGS) $(LDFLAGS) src/main.cpp src/render.cpp -o $(NAME) 

run:
	./$(NAME)

test:
	cd debug; \
	$(CC) -I../lib/ -framework OpenGL -lsdl2 ../lib/glad/glad.cpp ../src/*.cpp -g

compile_glad:
	$(CC) $(CFLAGS) -dynamiclib -o libglad.dylib lib/glad/glad.cpp
