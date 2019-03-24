#include "select_server.h"

#define BUFF_SIZE 100

int main(int argc, char **argv)
{
	int sock_fd;
	struct sockaddr_in addr;

	//1. creat socket
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0)
		err_pri("socket");

	//2 init ip port and address
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;
	// setsockopt
//	int b_resuse = 1;
//	setsockopot(sock_fd, SOL_SOCKET,SO_REUSEADDR, &b_resuse, sizeof(b_resuse));
	
	//3. bind
	if(bind(sock_fd, (struct sockaddr *)&addr,sizeof(addr)) < 0)
	{
		err_pri("bind:");
	}
	//4. listen
	if(listen(sock_fd, 5) < 0)
	{
		err_pri("listen");
	}
	printf("start listenning...\n");
	

	char buff[BUFF_SIZE];
	int read_ret;
	
	//5. waitting for connect
	int connect_fd;
	struct sockaddr_in cin;
	socklen_t len = sizeof(cin);

	connect_fd = accept(sock_fd, \
	(struct sockaddr *)&cin, &len);
	printf("connect fd = %d\n");	
	if(connect_fd < 0)
	{
		err_pri("accept:");
	}
	printf("%s client has been estabilished\n", cin.sin_addr);

	while(1)
	{
		bzero(buff, BUFF_SIZE);
		read_ret = read(sock_fd, buff, (BUFF_SIZE -1));
		if(0 == read_ret)
			printf("client disconnect...\n");
		else if(read_ret < 0)
			err_pri("read error:");
		else
			printf("%s\n", buff);

		if(0 == strncasecmp(buff, "quit", 4))
			break;
	}

	close(sock_fd);
	return 0;

}
