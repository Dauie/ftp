#ifndef CLIENT_H
#define CLIENT_H

#include "ftp.h"

# define CMD_CNT 8

char	*g_cmds[][2] = {
		{ "CWD ", "cd" },
		{ "HELP", "help" },
		{ "LIST", "ls" },
		{ "PASV", "passive" },
		{ "PWD", "pwd" },
		{ "QUIT", "quit" },
		{ "RETR", "get" },
		{ "STOR", "put" },
};

int 	c_cwd(t_session *session);
int 	c_help(t_session *session);
int 	c_list(t_session *session);
int 	c_passive(t_session *session);
int		c_pwd(t_session *session);
int		c_quit(t_session *session);
int		c_retrieve(t_session *session);
int 	c_store(t_session *session);

#endif