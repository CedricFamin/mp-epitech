CXXFLAGS	=	-g -W -Wall -Wextra -Wunused-parameter -std=c++0x

CXX		=	g++

LIBS		=	-lpthread

SRCS		=	main.cpp


OBJS		=	$(SRCS:.cpp=.o)

RM		=	rm -f

NAME		=	Vector

all		: 	$(NAME)

$(NAME)		:	$(OBJS)
		$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

.cpp.o		:
		$(CXX) $(CXXFLAGS) -c $<

depend		: 
		makedepend $(SRCS)

clean		:
		$(RM) $(OBJS)
		@$(RM) *~

fclean		:	clean
		$(RM) $(NAME)

re		:	fclean all

.PHONY		:	all .cpp.o depend clean fclean re
