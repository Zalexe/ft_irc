# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/03 16:56:12 by cmarrued          #+#    #+#              #
#    Updated: 2026/03/10 17:04:08 by intherna         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS = main.cpp src/server/Server.cpp src/server/Client.cpp src/server/Channel.cpp src/server/handles/handleRegistration.cpp src/server/handles/handleInvite.cpp \
		src/server/handles/handleJoin.cpp src/server/handles/handleKick.cpp src/server/handles/handleMode.cpp src/server/handles/handlePrivmsg.cpp \
		src/server/handles/handleQuit.cpp src/server/handles/handleTopic.cpp src/utils/Messages.cpp src/utils/Utils.cpp \
		src/server/handles/handleWho.cpp

INC = includes/

OBJS = ${SRCS:.cpp=.o}

DEPS = ${SRCS:.cpp=.d}

CPLUS	= c++

CFLAGS	= -Wall -Wextra -Werror -std=c++98 --pedantic -MMD# -g -fsanitize=address

RM	= rm -rf
			
			
all:			${NAME} 

%.o: %.cpp 	${INC}
			${CPLUS} ${CFLAGS} -c $< -o $@ -I$(INC)

${NAME}:	Makefile ${OBJS}
			${CPLUS} ${CFLAGS} ${OBJS} -o ${NAME}
							
clean:			
			${RM} ${OBJS} ${DEPS}

fclean:			clean
			${RM} ${NAME}
			

re:		fclean all

.PHONY:			all clean fclean re

-include ${DEPS}
