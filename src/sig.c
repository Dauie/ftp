
void 	handel_sig(int sig)
{
	pid_t pid;

	pid = 0;
	pid = wait4(pid, NULL, 0, NULL);
	return ;
}
