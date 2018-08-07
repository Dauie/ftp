#ifndef SERVER_H
#define SERVER_H

#include "ftp.h"
#include <sys/stat.h>

t_session *g_session;

typedef enum 			e_ephem
{
	EPHEM_MIN = 49152,
	EPHEM_MAX = 64738
}						t_ephem;

int			accept_connection(t_session *session, struct sockaddr_in check);
int			create_endpoint(t_session *session, char *address);
void		manage_client_session(t_session *session);
int			prep_send(t_session *session);
int			redirect_output_fd(int fd);
int			s_cwd(t_session *session);
int			s_help(t_session *session);
int			s_list(t_session *session);
int			s_passive(t_session *session);
int			s_port(t_session *session);
int			s_pwd(t_session *session);
int			s_quit(t_session *session);
int			s_store(t_session *session);
int			s_retrieve(t_session *session);

#endif
