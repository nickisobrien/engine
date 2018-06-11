NAME = engine
RM = /bin/rm -f

FLAGS = -Wall -Wextra -Werror

INCLUDE = -I ./includes/
SRC_DIR := ./srcs/
OBJ_DIR := ./objs/
LIB_DIR := ./lib/

HEADERS = ./includes/engine

# engine
FILES = main
CFILES = $(patsubst %, $(SRC_DIR)%.cpp, $(FILES))
OFILES = $(patsubst %, $(OBJ_DIR)%.o, $(FILES))

GL_FLAGS = -lglfw3 -framework AppKit -framework OpenGL -framework IOKit -framework CoreVideo
GL_DIR = $(LIB_DIR)glfw/src
GL_INC = -I $(LIB_DIR)glfw/include/
GL_LINK = -L$(GL_DIR) 

.PHONY: all clean fclean re

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo [INFO] engine Object Files Directory Created

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HEADERS).h
	g++ $(FLAGS) $(INCLUDE) $(GL_INC)  -o $@ -c $< 

$(NAME): $(OBJ_DIR) $(OFILES)
	@g++ $(FLAGS) $(GL_LINK) $(OFILES) $(GL_FLAGS) -o $(NAME)
	@echo [INFO] engine Binary Created

clean:
	@rm -rf $(OBJ_DIR)
	@echo [INFO] engine Object Files Directory Destroyed

fclean: clean
	@$(RM) $(NAME)
	@echo [INFO] engine Binary Destroyed

re: fclean all