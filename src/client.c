#include "../incl/ftp.h"
/*
 *  ./client
 */

static void usage(char *str)
{
	printf("Usage: %s <addr> <port>\n", str);
	exit(-1);
}

static void	client_shell(int client_socket)
{
	char	*buffer;
	char	recvbuff[BUFFSZ];
	ssize_t	ret;

	buffer = NULL;
	ret = 0;
	while (TRUE)
	{
		write(1, "(^-^)> ", 7);
		if (gnl(0, &buffer) < 0)
			break;
		send(client_socket, buffer, ft_strlen(buffer), 0);
		if (ft_strncmp(buffer, "quit", 4) == 0)
			break;
		while (TRUE)
		{
			if ((ret = recv(client_socket, recvbuff, BUFFSZ, MSG_DONTWAIT)) == -1)
				continue;
			else if (ret > 0)
			{
				recvbuff[BUFFSZ - 1] = '\0';
				printf("%s\n", recvbuff);
                continue;
			}
			else if (ret == 0)
                break;
		}
		ft_bzero(recvbuff, BUFFSZ);
		free(buffer);
	}
	printf("[-]Disconnected from server\n");
}

static int create_client(char *addr, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return(-1);
	if (!(sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)))
	{
		printf("[-]Error establishing socket (-.-)\n");
		exit(1);
	}
	printf("[+]Client socket created\n");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		printf("[-]Error connecting to %s:%d ...(-.-)\n", addr, port);
		exit(2);
	}
	printf("[+]Successfully connected to %s:%d\n", addr, port);
	return (sock);
}

int	main(int ac, char **av)
{
	int					port;
	int 				sock;

	if (ac != 3)
		usage(av[0]);
	port = atoi(av[2]);
	sock = create_client(av[1], port);
	client_shell(sock);
	close(sock);
	return(0);
}
