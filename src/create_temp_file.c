#include "../incl/ftp.h"

int 			create_temp_file(t_session *session)
{
	char			*tempfile;
	char 			*tmp;

	if (!(tempfile = ft_itoa(session->cs)))
		return (EXIT_FAIL);
	tmp = tempfile;
	if (!(tempfile = ft_strconcat("/tmp/", tmp)))
		return (EXIT_FAIL);
	free(tmp);
	tmp = tempfile;
	if (!(tempfile = ft_strconcat(tempfile, "sock.tmp")))
		return (EXIT_FAIL);
	free(tmp);
	if ((session->fd = open(tempfile, O_RDWR|O_CREAT)) == -1)
	{
		printf("[-]Error creating temp file.\n");
		return (EXIT_FAIL);
	}
	free(tempfile);
	return (EXIT_SUCCESS);
}
