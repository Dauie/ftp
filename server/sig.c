#include "../incl/server.h"

void 	handel_sig(int sig)
{
	pid_t pid;

	pid = 0;
	wait4(pid, NULL, 0, NULL);
}
