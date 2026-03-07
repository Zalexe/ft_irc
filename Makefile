# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/03 16:56:12 by cmarrued          #+#    #+#              #
#    Updated: 2026/03/07 15:28:16 by intherna         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserver

SRCS = src/main.cpp src/server/Server.cpp src/server/Client.cpp src/server/Channel.cpp src/server/handles/handleRegistration.cpp 

INC = includes/.hpp

OBJS = ${SRCS:.cpp=.o}

DEPS = ${SRCS:.cpp=.d}

CPLUS	= c++

CFLAGS	= -Wall -Wextra -Werror -std=c++98 --pedantic -MMD

RM	= rm -rf
			
			
all:			${NAME} 

%.o: %.cpp
			${CPLUS} ${CFLAGS} -c $< -o $@

${NAME}:	Makefile ${OBJS}
			${CPLUS} ${CFLAGS} ${OBJS} -o ${NAME}
							
clean:			
			${RM} ${OBJS}

fclean:			clean
			${RM} ${NAME}
			

re:		fclean all

.PHONY:			all clean fclean re

-include ${DEPS}
