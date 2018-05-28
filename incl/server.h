#ifndef SERVER_H
#define SERVER_H

#include "ftp.h"

# define MAX_CLIENTS (42)

typedef enum 			e_ephem
{
	EPHEM_MIN = 49152,
	EPHEM_MAX = 64738
}						t_ephem;


int			accept_connection(t_session *session);
int			change_dir(t_session *session);
int     	create_endpoint(t_session *session);

int 		create_temp_file(t_session *session);
int 		help(t_session *session);
void 		handel_sig(int sig);
int			list(t_session *session);
int  		passive(t_session *session);
int 		port(t_session *session);
int			prep_send(t_session *session);
int 		print_pwd(t_session *session);
int			quit(t_session *session);
int			redirect_output_fd(int fd);
int			send_client_msg(t_session *session, int n, ...);

char	*g_sprtd_cmds[] = { "CWD", "HELP", "LIST", "PASV", "PWD", "RETR", "STOR", "QUIT" };

int 	(*g_cmds[])(t_session *) = { &change_dir, &help, &list, &passive, &print_pwd, &send_file, &recv_file, &quit };

#endif
