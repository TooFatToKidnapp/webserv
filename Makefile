NAME = webserv

SRC =	./main.cpp\
		./src/parsing/ConfigFileParser.cpp\
		./src/parsing/utils.cpp\
		./src/parsing/LocationContext.cpp\
		./src/parsing/ConfigValues.cpp\
		./src/parsing/ServerContext.cpp\
		./src/parsing/ClientMaxBodySize.cpp\
		./src/parsing/Root.cpp\
		./src/parsing/Index.cpp\
		./src/parsing/ReturnDir.cpp\
		./src/parsing/UriParser.cpp\
		./src/parsing/Uri.cpp\
		./src/parsing/TargetRequestParser.cpp\
		./src/parsing/ServerName.cpp\
		./src/parsing/ParseCGI.cpp\
		./src/parsing/LocationUri.cpp\
		./src/parsing/Listen.cpp\
		./src/parsing/CheckMethods.cpp\
		./http/Request.cpp\
		./http/Response.cpp\
		./http/Socket.cpp\
		./http/Utils.cpp


FLAGS = -Wall -Werror -Wextra -std=c++98 -g -fsanitize=address

CC = c++

INC =	./headers/parsing/ConfigFileParser.hpp\
		./headers/parsing/utils.hpp\
		./headers/parsing/LocationContext.hpp\
		./headers/parsing/ConfigValues.hpp\
		./headers/parsing/ServerContext.hpp\
		./headers/parsing/ClientMaxBodySize.hpp\
		./headers/parsing/Root.hpp\
		./headers/parsing/Index.hpp\
		./headers/parsing/ReturnDir.hpp\
		./headers/parsing/UriParser.hpp\
		./headers/parsing/Uri.hpp\
		./headers/parsing/TargetRequestParser.hpp\
		./headers/parsing/ServerName.hpp\
		./headers/parsing/ParseCGI.hpp\
		./headers/parsing/LocationUri.hpp\
		./headers/parsing/Listen.hpp\
		./headers/parsing/HostStateParser.hpp\
		./headers/parsing/CheckMethods.hpp\
		./http/Request.hpp\
		./http/Response.hpp\
		./http/Utils.hpp\
		./http/codes.hpp


OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o : %.cpp $(INC)
	$(CC) $(FLAGS) -o $@ -c $<

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all
