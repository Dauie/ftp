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


# define DEFAULT_ADDRESS "127.0.0.1"
# define BUFFSZ (512)
# define HDRSZ (10)

typedef enum 	e_mode
{
	M_NON = 0,
	M_PSV = 1,
	M_PRT = 2
}				t_mode;

typedef enum    e_type
{
    T_SVR = 0,
    T_CLI = 1,
}               t_type;

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
	char                **env;
	char 				**argv;
	struct s_session	*psv;
}						t_session;


int			add_header(off_t size, char *buff);
int 		bind_socket(t_session *session);
void 		clean_session(t_session *session);
void        close_passive(t_session *session, t_type type);
int			create_socket(t_session *session, char *address);
void 		init_session(t_session * session);
int 		listen_socket(t_session *session);
int 		options_socket(t_session *session);
int			recv_file(int sock, int fd, char *buff, off_t len);
int 		recv_msg(int sock, char *buff, int *run_status);
int			send_file(int sock, int fd, char *buff, off_t len);
int			send_msg(int sock, int n, ...);
off_t       get_file_size(char *file);

#endif