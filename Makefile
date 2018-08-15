CC = gcc

CCFLAGS = -Wall -Werror -Wextra -g -fsanitize=address -O0

SVR_SRC =	src_server/server.c src_server/s_retrieve.c src_server/s_dir_cmd.c
SVR_SRC +=	src_server/s_client.c src_server/s_passive.c src_server/s_store.c
SVR_SRC +=  src_server/s_cwd.c src_server/s_help.c

CLI_SRC =	src_client/c_client.c src_client/c_dir_cmd.c src_client/c_passive.c
CLI_SRC +=	src_client/c_retrieve.c src_client/c_store.c src_client/c_help_useage_quit.c

CMN_SRC =	src_common/session.c src_common/recvfile.c src_common/sendfile.c
CMN_SRC +=	src_common/socket.c src_common/sig.c src_common/is_file.c

OBJ = *.o

RM = -rm -fr

all:
		$(MAKE) re -C ./libft/
		$(CC) $(CCFLAGS) $(CLI_SRC) $(CMN_SRC) ./libft/libftprintf.a -o client
		$(CC) $(CCFLAGS) $(SVR_SRC) $(CMN_SRC) ./libft/libftprintf.a -o server

libft:
		$(MAKE) re -C ./libft/

client:
		$(CC) $(CCFLAGS) $(CLI_SRC) $(CMN_SRC) ./libft/libftprintf.a -o client
server:
		$(CC) $(CCFLAGS) $(SVR_SRC) $(CMN_SRC) ./libft/libftprintf.a -o server
clean:
		$(RM) $(OBJ)

fclean: clean
		$(MAKE) fclean -C libft/
		$(RM) server
		$(RM) client
		$(RM) server.dSYM
		$(RM) client.dSYM

re: fclean all