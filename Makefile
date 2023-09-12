NAME = Renderer
LDFLAGS = -Iextern/ -framework OpenGL -lsdl2 -L. -lglad
CFLAGS = -std=c17 -O1 -Wall \
	-Wno-missing-braces \
	-Wno-unused-variable
CC = cc

.PHONY: all compile compile_t run test debug

all: compile run

compile:
	@echo "Compiling..."
	@$(CC) $(CFLAGS) $(LDFLAGS) src/main.c src/base_math.c src/render.c -o $(NAME)

compile_t:
	@echo "Compiling..."
	@time $(CC) $(CFLAGS) $(LDFLAGS) src/*.c -o $(NAME) 

run:
	./$(NAME)

test:
	@echo "Compiling test..."
	@$(CC) $(CFLAGS) src/test.c src/base_math.c  -o Test
	./Test

# debug:
# 	cd debug; \
# 	$(CC) -I../lib/ -framework OpenGL -lsdl2 ../lib/glad/glad.cpp ../src/*.c -g

compile_glad:
	$(CC) $(CFLAGS) -dynamiclib -o libglad.dylib extern/glad/glad.c
