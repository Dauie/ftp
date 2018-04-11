#include "../incl/ftp.h"

void init_session(t_session * session)
{
	session->port = 0;
	session->sock = 0;
	session->cs = 0;
	session->pid = 0;
	session->off = 0;
	session->size = 0;
	session->cslen = 0;
	session->env = NULL;
	session->argv = NULL;
}
