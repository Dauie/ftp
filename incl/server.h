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
int     	create_endpoint(t_session *session, char *address);
void 		handel_sig(int sig);
void     	manage_client_session(t_session *session);
int			prep_send(t_session *session);
int			redirect_output_fd(int fd);
int			s_cwd(t_session *session);
int 		s_help(t_session *session);
int			s_list(t_session *session);
int  		s_passive(t_session *session);
int 		s_port(t_session *session);
int 		s_pwd(t_session *session);
int			s_quit(t_session *session);
int			send_client_msg(t_session *session, int n, ...);





#endif
