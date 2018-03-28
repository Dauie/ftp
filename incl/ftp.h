#ifndef FTP_H
#define FTP_H

#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include "../libft/incl/str.h"
#include "../libft/incl/mem.h"
#include "../libft/incl/cnvrsn.h"
#include "../libft/incl/gnl.h"


# define CMD_PORT 4222
# define DATA_PORT 20
# define MAX_CLIENTS 42

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