# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/03 16:56:12 by cmarrued          #+#    #+#              #
#    Updated: 2025/12/03 16:56:13 by cmarrued         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserver

SRCS = src/main.cpp 

INC = includes/.hpp

OBJS = ${SRCS:.cpp=.o}

CPLUS	= c++

CFLAGS	= -Wall -Wextra -Werror -std=c++98 --pedantic

RM	= rm -rf
			
			
all:			${NAME} 

%.o: %.cpp 	$(INC)	
			${CPLUS} ${CFLAGS} -c $< -o $@

${NAME}:	Makefile ${OBJS}
			${CPLUS} ${CFLAGS} ${OBJS} -o ${NAME}
							
clean:			
			${RM} ${OBJS}

fclean:			clean
			${RM} ${NAME}
			

re:		fclean all

.PHONY:			all clean fclean re