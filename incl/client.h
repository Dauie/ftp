#ifndef CLIENT_H
#define CLIENT_H

#include "ftp.h"

# define CMD_CNT 8


int 	c_dircmd(t_session *session);
int 	c_help(t_session *session);
int 	c_list(t_session *session);
int 	c_passive(t_session *session);
int		c_quit(t_session *session);
int		c_retrieve(t_session *session);
int 	c_store(t_session *session);
int 	create_connection(t_session *session, char *addr);

#endif