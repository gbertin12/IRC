NAME = ircserv
BOTNAME = ircbot
CXXFLAGS = -Wall -Wextra -Werror -g3 -MMD -std=c++98 -gdwarf-4
CXX = c++
RM = rm -f
SRCS =	srcs/main.cpp \
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
BOTSRC =	srcs/bot/main.cpp \
			srcs/bot/utils.cpp \
			srcs/bot/bot.cpp
DEPENDS := $(patsubst %.cpp,%.d,$(SRCS))

OBJ = $(SRCS:.cpp=.o)
BOTOBJS = $(BOTSRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CXX) $(CXXFLAGS)  -o $(NAME) $(OBJ)

$(BOTNAME): $(BOTOBJS)
		$(CXX) $(CXXFLAGS)  -o $(BOTNAME) $(BOTOBJS)

clean:
		$(RM) $(OBJ)
		$(RM) $(OBJ) $(DEPENDS)
		$(RM) $(BOTOBJS)
		
fclean:	clean
		$(RM) $(OBJ)
		$(RM) $(NAME)
		$(RM) $(OBJ) $(DEPENDS)
		$(RM) $(BOTNAME)
		$(RM) $(BOTOBJS)

re: fclean all

re bonus: fclean bonus

bonus: $(NAME) $(BOTNAME)

-include $(DEPENDS)

.PHONY: all clean fclean re bonus