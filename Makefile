NAME = webserv

SRC =	main.cpp\
		./src/ConfigFileParser.cpp\

FLAGS = -Wall -Werror -Wextra -Wshadow -std=c++98 -fsanitize=address

CC = c++

INC = ./headers/ConfigFileParser.hpp\

OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(INC)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o : %.cpp $(INC)
	$(CC) $(FLAGS) -o $@ -c $<

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all
