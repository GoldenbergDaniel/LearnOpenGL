NAME = Renderer
CC = cc

CFLAGS = -Ilib/ -std=c17 -O1 -Wall \
				 -Wno-missing-braces \
				 -Wno-unused-variable \
				 -Wno-unused-function

LDFLAGS = -framework OpenGL \
					-lsdl2 \

LIB = lib/glad/glad.c \

SRC = src/main.c \
			src/base_math.c \
			src/render.c

.PHONY: all compile compile_t run test debug

all: compile run

compile:
	@echo "Compiling..."
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LIB) $(SRC) -o $(NAME)

compile_t:
	@echo "Compiling..."
	@time $(CC) $(CFLAGS) $(LDFLAGS) $(LIB) $(SRC) -o $(NAME) 

run:
	./$(NAME)

test:
	@echo "Compiling test..."
	@$(CC) $(CFLAGS) test/test_math.c src/base_math.c -o TestMath
	./TestMath

debug:
	cd debug; \
	$(CC) -I../lib/ $(LDFLAGS) ../lib/glad/glad.c ../src/*.c -g
