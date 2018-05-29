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

#include "../libft/incl/str.h"
#include "../libft/incl/mem.h"
#include "../libft/incl/cnvrsn.h"
#include "../libft/incl/gnl.h"


# define BUFFSZ (1024)
# define HDRSZ (10)

typedef enum 	e_mode
{
	M_NON = 0,
	M_PSV = 1,
	M_PRT = 2
}				t_mode;

char	*g_sprtd_cmds[] = { "CWD", "HELP", "LIST", "PASV", "PWD", "QUIT" , "RETR", "STOR"};

typedef struct          s_session
{
	int 				run;
	int 				mode;
	int                 port;
	int                 sock;
	int                 cs;
	int 				fd;
	int 				opt;
	pid_t 				pid;
	unsigned int        cslen;
	struct sockaddr_in  csin;
	struct sockaddr_in  sin;
	char 				buff[BUFFSZ];
	char                **env; // maybe remove
	char 				**argv;
	struct s_session	*psv;
}						t_session;


int			add_header(off_t size, char *buff);
int 		bind_socket(t_session *session, char *address);
int			create_socket(t_session *session);
int 		create_temp_file(t_session *session);
void 		init_session(t_session * session);
int 		listen_socket(t_session *session);
int 		options_socket(t_session *session);
int 		recv_file(t_session *session);
int 		recv_msg(t_session *session);
int			send_file(t_session *session);
int			send_msg(t_session *session, int n, ...);

#endif