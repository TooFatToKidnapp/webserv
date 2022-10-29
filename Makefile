NAME = webserv

SRC =	main.cpp\
		./src/ConfigFileParser.cpp\
		./src/LocationContext.cpp\
		./src/ServerContext.cpp\


FLAGS = -Wall -Werror -Wextra -std=c++98

CC = c++

INC = ./headers/ConfigFileParser.hpp\
		./headers/LocationContext.hpp\
		./headers/ServerContext.hpp\

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
