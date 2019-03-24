#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define PORT 		5003
#define SER_IP		192.168.7.75
#define QUIT_STR	"quit"


void usage_print(char *ss)
{
	printf("Usage:\n");
	printf("\t %s <server_ip> <server_port>\n",ss);
	exit(1);
}

void error_print(char *ss)
{
	perror(ss);
	exit(1);
}


int main(int argc, char *argv[])
{
	int port = PORT;
	struct sockaddr_in addr;
	int sock_fd;

	if(argc < 2)
		usage_print(argv[0]);

	port = atoi(argv[2]);
	if(port < 5000 || port > 65535)
		usage_print(argv[0]);

	//1. create socket
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0)
		error_print("socket");

	//2. set ip and port
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;		// connect mode
	addr.sin_port = htons(port);	// connect port
	addr.sin_addr.s_addr = inet_addr(argv[1]);	//connect ip

	//3.connect server

	if(connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)	
		error_print("connect");

	printf("process [%d] connect established...\n", getpid());

	//4. communication
	int write_ret;
	char buff[300];
	int buff_size;
	buff_size = sizeof(buff)/sizeof(char);

	while(1)
	{
		fprintf(stdout,"please input chara: ");
		bzero(buff, buff_size);
		fgets(buff, buff_size -1, stdin);
		do
		{
			write_ret = write(sock_fd, buff, strlen(buff));
		}while((write_ret < 0) && (EINTR == errno));
		//check quit flage
		if(!strncasecmp(buff, QUIT_STR, strlen(QUIT_STR)))
			break;
	}
	
	//5. quit
	close(sock_fd);

	return 0;
}







