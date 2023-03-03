NAME = ircserv
CXXFLAGS = -Wall -Wextra -Werror -g3 -MMD -std=c++98 -gdwarf-4
CXX = c++
RM = rm -f
SRCS =	main.cpp \
		srcs/modes/ChannelModes.cpp \
		srcs/modes/UserModes.cpp \
		srcs/modes/PrivilegesModes.cpp \
		srcs/Client.cpp \
		srcs/Channel.cpp \
		srcs/Server.cpp \
		srcs/Command/Command.cpp \
		srcs/Command/CommandAuthentification.cpp \
		srcs/Command/CommandChannel.cpp \
		srcs/Command/CommandServer.cpp \
		srcs/Command/CommandUser.cpp \
		srcs/utils.cpp
		
DEPENDS := $(patsubst %.cpp,%.d,$(SRCS))

OBJ = $(SRCS:.cpp=.o)


all: $(NAME)

$(NAME): $(OBJ)
		$(CXX) $(CXXFLAGS)  -o $(NAME) $(OBJ)

clean:
		$(RM) $(OBJ)
		$(RM) $(OBJ) $(DEPENDS)
		
fclean:	clean
		$(RM) $(OBJ)
		$(RM) $(NAME)
		$(RM) $(OBJ) $(DEPENDS)

re: fclean all

-include $(DEPENDS)

.PHONY: all clean fclean re