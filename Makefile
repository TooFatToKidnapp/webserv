NAME = webserv

FLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = Request.cpp Response.cpp Socket.cpp Utils.cpp main.cpp

HDRS = Request.hpp Response.hpp Socket.hpp Utils.hpp

OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	c++ $(FLAGS) $^ -o $@

%.o : %.cpp $(HDRS)
	c++ $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re