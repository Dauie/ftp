#ifndef FTP_H
#define FTP_H

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../libft/incl/str.h"
#include "../libft/incl/mem.h"
#include "../libft/incl/cnvrsn.h"
#include "../libft/incl/gnl.h"

# define BUFFSZ 1024
# define HDRSZ 10
# define CMD_PORT 4222
# define DATA_PORT 20
# define MAX_CLIENTS 42
# define EXIT_FAIL -1
# define EXIT_SUCCESS 0

char	g_cmds[] = {"CWD", "HELP", "LIST", "PASV", "PWD", "RETR", "STOR", "QUIT"};

typedef struct          s_session
{
    int                 port;
    int                 sock;
    int                 cs;
	int 				fd;
	int 				opt;
	pid_t 				pid;
	off_t 				off;
	off_t 				size;
    unsigned int        cslen;
    struct sockaddr_in  csin;
	struct sockaddr_in  sin;
    char 				buff[BUFFSZ];
    char                **env;
	char 				**argv;
}                       t_session;

int			add_header(off_t size, char *buff);
int 		bind_socket(t_session *session);
int			change_dir(t_session *session, char *dir);
int			create_socket(t_session *session);
int 		create_temp_file(t_session *session);
int 		ftp_recvfile(t_session *session);
int			ftp_sendfile(t_session *session);
void 		init_session(t_session * session);
int			list(t_session *session);
int 		listen_socket(t_session *session);
int 		options_socket(t_session *session);
int			prep_send(t_session *session);
int 		print_cwd(t_session *session);
int			redirect_output_fd(int fd);

#endif
