CXXFLAGS	=	-g -W -Wall -Wextra -Wunused-parameter -std=c++11 -O3 -pg

CXX		=	g++

LIBS		=

SRCS		=	sources/main.cpp		\
			sources/htm.cpp			\
			sources/htmasciiparser.cpp	\
			sources/logservice.cpp		\
			sources/trixel.cpp

OBJS		=	$(SRCS:.cpp=.o)

RM		=	rm -f

NAME		=	Parallel_comp

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
