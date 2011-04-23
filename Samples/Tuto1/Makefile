
NAME	=	Tuto1

SRC	=	main.cpp \
		GameEngine.cpp

OBJ	=	$(SRC:.cpp=.o)

RM	=	rm -rf

CXX	=	g++

LIB	=	-L../../lib/ -lNc-Core -lNc-Graphics -lGL

INCLUDE	=	-I../../

CFLAGS	=	-W -Wall $(INCLUDE)

.cpp.o	:
		$(CXX) -c $(CFLAGS) $< -o $(<:.cpp=.o)

all	:	$(OBJ)
		$(CXX) -o $(NAME) $(OBJ) $(LIB)
		chmod 705 $(NAME)
		chmod 604 $(SRC) $(OBJ)

clean	:
		$(RM)	$(OBJ)
		$(RM)	*~ *# *.core

fclean	:	clean
		$(RM) $(NAME)

re	:	clean all
