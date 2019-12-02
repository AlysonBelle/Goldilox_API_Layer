NAME		= RT

SRC_PATH	= src/
OBJS_PATH	= objs/

SRC = camera.cpp \
		sphere.cpp \
		plane.cpp \
		color.cpp \
		image.cpp \
		ray.cpp \
		SDL.cpp \
		shape.cpp \
		vectormath.cpp \
		octivation.cpp \
		main.cpp \
		light.cpp \
		tracer.cpp

OBJ	=	$(SRC:.cpp=.o)

SRCS =	$(addprefix $(SRC_PATH), $(SRC))
OBJS =	$(addprefix $(OBJS_PATH), $(OBJ))

HEAD =	-I hds/

SDLINC	= `sdl2-config --cflags`
SDLLIB	= `sdl2-config --libs`
CC		= g++

ifeq ($(OS),Windows_NT)
	SDLINC	= -I C:/SDL2/include
	SDLLIB	= -L C:/SDL2/lib/x64 -lSDL2main -lSDL2
	CC		= x86_64-w64-mingw32-g++
endif

all: $(NAME)

$(NAME): $(OBJS)
	@echo "------- COMPILLING --------"
	@$(CC) -o $(NAME) $(OBJS) $(SDLLIB) -std=c++11
	@echo "-------  COMPILED  --------"

$(OBJS_PATH)%.o: $(SRC_PATH)%.cpp
	@echo "\033[0;32m------- COMPILLING $@ --------\033[0m"
	@mkdir -p $(OBJS_PATH)
	@$(CC) -o $@ -c $< $(HEAD) $(SDLINC) -std=c++11

clean:
	@rm -f $(NAME)

fclean: clean
	@rm -f $(OBJS)
	@rm -rf $(OBJS_PATH)

re: fclean all
