#include <errno.h>
#include "../incl/ftp.h"

void    usage(char *str)
{
    printf("Useage: %s <port>\n", str);
    exit(-1);
}

int     create_server(int port)
{
    int sock;
	int opt = TRUE;
    struct protoent *proto;
    struct sockaddr_in  sin;

    sock = 0;
    proto = getprotobyname("tcp");
    if (proto == 0)
        return (-1);
    if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		printf("[-]Error creating socket\n");
		exit(-1);
	}
    sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		printf("[-]Error setting options on master socket %d", sock);
	}
    if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        printf("[-]Error binding socket %d\n", sock);
        exit(-1);
    }
    if (listen(sock, 4) == 0)
        printf("[+]Awaiting request...\n");
    else
        printf("[-]Error in binding port %d may be in use\n", CMD_PORT);
    return (sock);
}

/*
 * Initialize all client sockets to 0
 * */
void	init_csockets(int *array, size_t length)
{
	while (length > 0)
		array[length] = 0;
}

void	add_child_sockets(int *max_sd, int len, int *sockets, fd_set *readfds)
{
	int i;

	i = -1;
	while (++i < len)
	{
		if (sockets[i] > 0)
			FD_SET(sockets[i], readfds);
		if (sockets[i] > *max_sd)
			*max_sd = sockets[i];
	}
}

void	add_active_sd(int *sockets, int sd, int length)
{
	int i;

	i = -1;
	while (++i < length)
	{
		if (sockets[i] == 0)
		{
			sockets[i] = sd;
			break;
		}
	}
}

int main(int ac, char **av)
{

    int port;

    int sock;
    int cs;
	int max_sd;
	int csockets[MAX_CLIENTS];
    unsigned int cslen;
    struct sockaddr_in  csin;
    ssize_t ret;
	fd_set readfds;
    char buff[1024];

    if (ac != 2)
        usage(av[0]);
    port = atoi(av[1]);
    sock = create_server(port);
	cslen = sizeof(csin);
    while (TRUE)
    {
		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		max_sd = sock;
		add_child_sockets(&max_sd, MAX_CLIENTS, (int*)&csockets, &readfds);
		if ((select( max_sd + 1 , &readfds , NULL , NULL , NULL)) < 0 && (h_errno != EINTR))
			printf("[-]Error with select()\n");

		if (FD_ISSET(sock, &readfds))
		{
			if (!(cs = accept(sock, (struct sockaddr*)&csin, (socklen_t*)&cslen)))
			{
				printf("[-]Error accepting connection.");
				break;
			}
			add_active_sd((int*)&csockets, cs, MAX_CLIENTS);
			printf("[+]New connection accepted from %s:%d on sd %d\n", inet_ntoa(csin.sin_addr), ntohs(csin.sin_port), cs);
		}
		else
		{
			int i = -1;
			while (++i < MAX_CLIENTS)
			{
				if (FD_ISSET(csockets[i], &readfds))
				{
					if (!(ret = read(csockets[i], buff, 1023)))
						continue;
					buff[ret] = '\0';
					if (ft_strcmp((const char *)&buff, "quit" ) == 0)
					{
						printf("[+]Host has disconnected from socket %d\n", csockets[i]);
						close(csockets[i]);
						csockets[i] = 0;
					}
					else
					{
						printf("%s", buff);
						send(csockets[i], buff, strlen(buff), 0);
					}
					ft_bzero(&buff, 1024);
				}
			}
		}
    }
	close(cs); // need to close all sockets.
    close(sock); //close master socket.
    return(0);
}
