
NAME	=	Demo

SRC	=	main.cpp \
		GameEngine.cpp

OBJ	=	$(SRC:.cpp=.o)

RM	=	rm -rf

CC	=	g++

LIB	=	-L../../lib/ -lNc-Core -lNc-Graphics -lNc-Contrib -lGL -lNc-GUI

INCLUDE	=	-I../../

CFLAGS	=	-W -Wall $(INCLUDE)

.cpp.o	:
		$(CC) -c $(CFLAGS) $< -o $(<:.cpp=.o) -D_DEBUG

all	:	$(OBJ)
		$(CC) -o $(NAME) $(OBJ) $(LIB)
		chmod 705 $(NAME)
		chmod 604 $(SRC) $(OBJ)

clean	:
		$(RM)	$(OBJ)
		$(RM)	*~ *# *.core

fclean	:	clean
		$(RM) $(NAME)

re	:	clean all
