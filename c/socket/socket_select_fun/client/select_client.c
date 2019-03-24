#include "select_client.h"

#define BUFF_SIZE 100

int main(int argc, char **argv)
{
	int sock_fd;
	struct sockaddr_in addr;

	if(2 != argc)
	{
		printf("Usage: %s <Server_IP>\n", argv[0]);
		exit(1);
	}
	
	
	
	//1. creat socket
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0)
		err_pri("socket");

	//2 init ip port and address
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	//3. connect server
	if(connect(sock_fd, (struct sockaddr *)&addr,sizeof(addr)) < 0)
	{
		err_pri("connect:");
	}
	//3. comunication
	char buff[BUFF_SIZE];
	while(1)
	{
		bzero(buff, BUFF_SIZE);
		fgets(buff, BUFF_SIZE, stdin);
		write(sock_fd, buff, (BUFF_SIZE -1));
		if(0 == strncasecmp(buff, "quit", 4))
			break;
	}

	close(sock_fd);
	return 0;

}
