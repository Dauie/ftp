#ifndef FTP_H
#define FTP_H

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../libft/incl/str.h"
#include "../libft/incl/mem.h"
#include "../libft/incl/cnvrsn.h"
#include "../libft/incl/gnl.h"

# define BUFFSZ 512
# define CMD_PORT 4222
# define DATA_PORT 20
# define MAX_CLIENTS 42
# define EXIT_FAIL -1
# define EXIT_SUCCESS 0

typedef struct          s_session
{
    int                 port;
    int                 sock;
    int                 cs;
	int 				fd;
	pid_t				pid;
    unsigned int        cslen;
    struct sockaddr_in  csin;
    char 				buff[1024];
    char                **env;
	char 				**argv;
}                       t_session;

int			ftp_sendfile(int fd, int sock, off_t *offset, off_t len);

#endif

/*
 *
 * FD_SET structure description

typedef struct 		fd_set
{
 	u_int  fd_count;
 	SOCKET fd_array[FD_SETSIZE]; (FD_SETSIZE == 1024)
} 					fd_set;

 **/