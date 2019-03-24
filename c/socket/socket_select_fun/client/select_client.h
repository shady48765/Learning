#ifndef __SELECT_CLIENT_H
#define __SELECT_CLIENT_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>




#define PORT  5003



#define err_pri(str) ({  \
			    perror(str);\
			    exit(1);\
			})






#endif
