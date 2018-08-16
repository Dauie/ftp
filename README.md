# FTP
## A small ftp server & client

### How to install:

`git clone https://github.com/dauie/ftp`

`cd ftp && git clone https://github.com/dauie/libft && make`

### How to run:

for server: `./server <port>`

for client: `./client <address> <port>`

### Current Features

`CWD `|`cd` `<dir>` - change working directory.

`HELP`|`help` - Lists all supported commands.

`LIST`|`ls` `<path>` - List files/directories in path.

`PASV`|`passive` - Enter passive mode.

`PWD`|`pwd` - Prints working directory.

`QUIT`|`quit` - Closes connection and quits program.

`RETR`|`get` `<path>` - Retrieve file at path.

`STOR`|`put` `<path>` - Store file at path.


![alt text](https://github.com/Dauie/ftp/blob/master/ftpss.png)
