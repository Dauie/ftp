#ifndef FTP_H
#define FTP_H

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "../libft/incl/net.h"
#include "../libft/incl/str.h"
#include "../libft/incl/mem.h"
#include "../libft/incl/cnvrsn.h"
#include "../libft/incl/return_types.h"

# define BUFFSZ (512)

typedef enum			e_mode
{
	M_NON,
	M_PSV,
	M_PRT,
}						t_mode;

typedef enum			e_type
{
	T_SVR,
	T_CLI,
}						t_type;

typedef struct			s_session
{
	int					run;
	volatile int 		kill;
	int					mode;
	int					port;
	int					sock;
	int					cs;
	int					fd;
	pid_t				pid;
	unsigned int		cslen;
	struct sockaddr_in	csin;
	struct sockaddr_in	sin;
	char				buff[BUFFSZ];
	char				**env;
	char				**argv;
	struct s_session	*psv;
}						t_session;

t_session 				*g_session;

int			bind_socket(t_session *session);
void		clean_session(t_session *session);
void		close_passive(t_session *session, t_type type);
int			create_socket(t_session *session, char *address);
void		handel_sig(int sig);
void		init_session(t_session * session);
void		kill_client(t_session *session);
void		kill_server(t_session *session);
int			listen_socket(t_session *session);
int			options_socket(t_session *session);
int			recv_file(int sock, int fd, char *buff, off_t len);
int			recv_msg(int sock, char *buff, int *run_status);
int			send_file(int sock, int fd, char *buff, off_t len);
int			send_msg(int sock, int n, ...);
off_t		get_file_size(char *file);

#endif