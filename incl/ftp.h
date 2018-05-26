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

# define BUFFSZ (1024)
# define CMD_PORT (4222)
#define EXIT_SUCCESS (0)
#define EXIT_FAIL (-1)
# define HDRSZ (10)
# define MAX_CLIENTS (42)

typedef enum 			e_ephem
{
	EPHEM_MIN = 49152,
	EPHEM_MAX = 64738
}						t_ephem;

typedef struct          s_session
{
	int 				run;
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
	struct s_session	*psv;
}                       t_session;



int			accept_connection(t_session *session);
int			add_header(off_t size, char *buff);
int 		bind_socket(t_session *session);
int			change_dir(t_session *session);
int     	create_endpoint(t_session *session);
int			create_socket(t_session *session);
int 		create_temp_file(t_session *session);
int 		help(t_session *session);
void 		handel_sig(int sig);
void 		init_session(t_session * session);
int			list(t_session *session);
int 		listen_socket(t_session *session);
int 		options_socket(t_session *session);
int  		passive(t_session *session);
int 		port(t_session *session);
int			prep_send(t_session *session);
int 		print_pwd(t_session *session);
int 		recvfile(t_session *session);
int			redirect_output_fd(int fd);
int			sendfile(t_session *session);
int			send_client_msg(t_session *session, char *code, char *msg);

char	*g_sprtd_cmds[] = {"CWD", "HELP", "LIST", "PASV", "PWD", "RETR", "STOR", "QUIT"};

int 	(*g_cmds[])(t_session *) = { &change_dir, &help, &list, &passive, &sendfile, &recvfile };

#endif
