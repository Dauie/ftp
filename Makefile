
CC = gcc

CCFLAGS = -Wall -Werror -Wextra -g -fsanitize=address -O0

SVR_SRC =  src/server.c

CLI_SRC = src/client.c

OBJ = *.o

RM = -rm -fr

all:
		$(MAKE) -C ./libft/ re
		$(CC) $(CCFLAGS) $(CLI_SRC) ./libft/libftprintf.a -o client
		$(CC) $(CCFLAGS) $(SVR_SRC) ./libft/libftprintf.a -o server


client:
		$(CC) $(CCFLAGS) $(CLI_SRC) ./libft/libftprintf.a -o client

server:
		$(CC) $(CCFLAGS) $(SVR_SRC) ./libft/libftprintf.a -o server
clean:
		$(RM) $(OBJ)

fclean: clean
		$(RM) server
		$(RM) client
		$(RM) server.dSYM
		$(RM) client.dSYM

re: fclean all
