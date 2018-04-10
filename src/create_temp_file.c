#include "../incl/ftp.h"

int 			create_temp_file(t_session *session)
{
	char			*tempfile;
	char 			*tmp;

	tempfile = ft_itoa(session->cs);
	tmp = tempfile;
	if (!(tempfile = ft_strcat("/tmp/", tmp)))
		return (EXIT_FAIL);
	free(tmp);
	tmp = tempfile;
	if (!(tempfile = ft_strcat(tempfile, "sock.tmp")))
		return (EXIT_FAIL);
	free(tmp);
	if ((session->fd = open(tempfile, O_RDWR|O_CREAT)) == -1)
	{
		printf("[-]Error creating temp file.\n");
		return (EXIT_FAIL);
	}
	return (EXIT_SUCCESS);
}