CC = gcc

CCFLAGS = -Wall -Werror -Wextra -g -fsanitize=address -O0

SVR_SRC =  src_server/server.c src_server/sig.c src_server/s_dir_cmd.c src_server/s_client.c \
			src_server/s_passive.c src_server/s_store.c src_server/s_retrieve.c

CLI_SRC = src_client/c_client.c src_client/c_dir_cmd.c src_client/c_passive.c \
			src_client/c_retrieve.c src_client/c_store.c

CMN_SRC = src_common/session.c src_common/recvfile.c src_common/sendfile.c \
			src_common/socket.c
OBJ = *.o

RM = -rm -fr

all:
		$(MAKE) -C ./libft/ re
		$(CC) $(CCFLAGS) $(CLI_SRC) $(CMN_SRC) ./libft/libftprintf.a -o client
		$(CC) $(CCFLAGS) $(SVR_SRC) $(CMN_SRC) ./libft/libftprintf.a -o server

client:
		$(CC) $(CCFLAGS) $(CLI_SRC) $(CMN_SRC) ./libft/libftprintf.a -o client
server:
		$(CC) $(CCFLAGS) $(SVR_SRC) $(CMN_SRC) ./libft/libftprintf.a -o server
clean:
		$(RM) $(OBJ)

fclean: clean
		$(RM) server
		$(RM) client
		$(RM) server.dSYM
		$(RM) client.dSYM

re: fclean all