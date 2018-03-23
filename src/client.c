#include "../incl/ftp.h"
/*
 *  ./client
 */

static bool is_ip_address(char *address)
{
	int i;

	i = -1;
	while (address[++i])
	{
		if (address[i] != '.' || !ft_isdigit(address[i]))
			return (false);
	}
	return (true);
}


static int verify_port(char *port)
{
	int ret;

	if (!port)
		return (-1);
	ret = ft_atoi(port);
	if (ret <= 0)
		return (-1);
	return (ret);
}

static void manage_host_connection_failure(char *address) {
	if (h_errno == HOST_NOT_FOUND)
		printf("[-]There was an issue finding '%s'\n", address);
	else if (h_errno == TRY_AGAIN)
		printf("[-]An error occurred. Try connecting to '%s' again\n", address);
	else if (h_errno == NO_RECOVERY)
		printf("[-]An unexpected error has occurred connecting to '%s'\n", address);
	else if (h_errno == NO_DATA)
		printf("[-]no IP address is associated with '%s'\n", address);
	if (h_errno == HOST_NOT_FOUND || h_errno == TRY_AGAIN || h_errno == NO_RECOVERY || h_errno == NO_DATA)
		exit(1);
}

static void verify_request(struct sockaddr_in *server_address, char *address, char *port)
{
	struct hostent *response;
	int	prt;

	prt = 0;
	response = NULL;
	ft_memset(server_address, 0, sizeof(server_address));
	if (is_ip_address(address))
		server_address->sin_addr.s_addr = inet_addr(address);
	else
	{
		response = gethostbyname(address);
		manage_host_connection_failure(address);
		server_address->sin_addr.s_addr = inet_addr(response->h_addr_list[0]);

	}
	prt = verify_port(port);
	if (port < 0)
	{
		prt = DATA_PORT;
		printf("[-]Invalid port number, defaulting to %i\n", prt);
	}
	server_address->sin_family = AF_INET;
	server_address->sin_port = htons(prt);
};

void	client_shell(int client_socket)
{
	char *buffer;

    buffer = NULL;
	while (1)
	{
		write(1, "(^-^)> ", 7);
		if (gnl(0, &buffer) < 0)
            break;
		if (ft_strcmp(buffer, "quit") == 0)
            break;
        send(client_socket, buffer, ft_strlen((const char *)&buffer), 0);
        if (recv(client_socket, buffer, 1024, 0) < 0)
            ft_printf("[-]Error receiving data from server (-.-)\n");
        else
            ft_printf("%s\n", buffer);
	}
    printf("[-]Disconnected from server\n");
    free(buffer);
    close(client_socket);
}

int		main(int ac, char **av)
{
	struct sockaddr_in server_address;
	int client_socket = 0;
	char *address;

	// Get socket to communicate on
	if (ac < 2)
	{
		printf("useage: ./client <address> <port>\n");
		exit(1);
	}
	verify_request(&server_address, av[1], av[2]);
	if (!(client_socket = socket(AF_INET, SOCK_STREAM, 0)))
	{
		printf("[-]Error establishing socket (-.-)\n");
		exit(1);
	}
	printf("[+]Client socket created\n");
	if (!(connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address))))
	{
		printf("[-]Error connecting to %s on port %s (-.-)\n", av[1], av[2]);
		exit(1);
	}
	printf("[+]Successfully connected to %s on port %s\n", av[1], av[2]);
	client_shell(client_socket);
}