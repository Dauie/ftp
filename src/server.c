#include "../incl/ftp.h"

int     main(void)
{
    struct  sockaddr_in server_address;
    struct  sockaddr_in new_address;
    char    buffer[1024];

    socklen_t addr_size;
    int new_socket = 0;
    int server_socket = 0;
    int child_pid = 0;


    // Get socket to communicate on
    if (!(server_socket = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("[-]Error establishing socket (-.-)\n");
        exit(1);
    }
    printf("[+]Server socket created\n");
    ft_memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CMD_PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server_socket, (const struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        printf("[-]Error binding server socket\n");
        exit(1);
    }
    printf("Socket bound to port %i\n", CMD_PORT);
    if (listen(server_socket, 10) == 0)
        printf("[+]Awaiting request...\n");
    else
        printf("[-]Error in binding\n");
    while (1)
    {
        new_socket = accept(server_socket, (struct sockaddr*)&new_address, &addr_size);
        if (new_socket < 0)
            exit(1);
        printf("[+]Connection accepted from %s:%d", inet_ntoa(new_address.sin_addr), ntohs(new_address.sin_port));
        if ((child_pid = fork()) == 0)
        {
            recv(new_socket, buffer, 1024, 0);
            if (ft_strcmp((const char *)&buffer, "quit" ))
            {
                printf("Host from %s:%d has disconnected\n", inet_ntoa(new_address.sin_addr), ntohs(new_address.sin_port));
                break;
            }
            else
            {
                printf("%s", buffer);
                send(new_socket, buffer, strlen((const char*)&buffer), 0);
                ft_bzero(&buffer, 1024);
            }
        }
    }
}