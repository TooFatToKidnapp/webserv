NAME = webserv

SRC =	./main.cpp\
		./src/ConfigFileParser.cpp\
		./src/utils.cpp\
		./src/LocationContext.cpp\
		./src/ConfigValues.cpp\
		./src/ServerContext.cpp\
		./src/ClientMaxBodySize.cpp\
		./src/Root.cpp\
		./src/Index.cpp\
		./src/ReturnDir.cpp\
		./src/UriParser.cpp\
		./src/Uri.cpp\
		./src/TargetRequestParser.cpp\
		./src/ServerName.cpp\
		./src/ParseCGI.cpp\
		./src/LocationUri.cpp\
		./src/Listen.cpp\
		./src/CheckMethods.cpp


FLAGS = -Wall -Werror -Wextra -std=c++98

CC = c++

INC =	./headers/ConfigFileParser.hpp\
		./headers/utils.hpp\
		./headers/LocationContext.hpp\
		./headers/ConfigValues.hpp\
		./headers/ServerContext.hpp\
		./headers/ClientMaxBodySize.hpp\
		./headers/Root.hpp\
		./headers/Index.hpp\
		./headers/ReturnDir.hpp\
		./headers/UriParser.hpp\
		./headers/Uri.hpp\
		./headers/TargetRequestParser.hpp\
		./headers/ServerName.hpp\
		./headers/ParseCGI.hpp\
		./headers/LocationUri.hpp\
		./headers/Listen.hpp\
		./headers/HostStateParser.hpp\
		./headers/CheckMethods.hpp


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
