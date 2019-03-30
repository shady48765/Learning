
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

#define PORT 		5003
#define SER_IP		192.168.7.75
#define QUIT_STR	"quit"

//#define SPECIFIC_IP   1

void usage_print(char *ss)
{
	printf("Usage:\n");
	printf("\t %s <server_ip>\n",ss);
	exit(1);
}

void error_print(char *ss)
{
	perror(ss);
	exit(1);
}


int main(int argc, char *argv[])
{
	struct sockaddr_in addr;
	int sock_fd, sock_new_fd;

#if SPECIFIC_IP
	if(argc < 2)
		usage_print(argv[0]);
#endif

	//1. create socket
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0)
		error_print("socket");

	//2. set port and ip can re-use quickly
	int b_reuse = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(b_reuse));

	//3. bind server ip and port
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;		// connect mode
	addr.sin_port = htons(PORT);	// connect port

#if SPECIFIC_IP
	// allow ip which equal input arg[1]
	addr.sin_addr.s_addr = inet_addr(argv[1]);
#else
	addr.sin_addr.s_addr = htons(INADDR_ANY);
#endif

	if(bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		error_print("bind");

	//4. changed socket to listen mode
	if(listen(sock_fd, 5) < 0)
		error_print("listen");

	printf("server startting...\n");

	int read_ret;
	char buff[300];
	int buff_size;
	pid_t fork_pid;
	while(1)
	{
		//5. waitting for connect...
		sock_new_fd = accept(sock_fd, NULL, NULL);
		if(sock_new_fd < 0)
			error_print("accept");
		printf("A new client is connection established. \n \
				ip is: (cannot get ip now)\n");

		fork_pid = fork();
		if(fork_pid < 0)
			error_print("fork");

		if(0 == fork_pid)
		{

			while(1)
			{

				//6. communication
				buff_size = sizeof(buff)/sizeof(char);

				bzero(buff, buff_size);
				do
				{
					read_ret = read(sock_new_fd, buff, (buff_size -1));
				}while((read_ret < 0) && (EINTR == errno));
				if(read_ret < 0)
				{
					perror("tread read");
					exit(0);
				}
				printf("process: (%d) received message is %s", getpid(), buff);

				if(!strncasecmp(buff, QUIT_STR, strlen(QUIT_STR)))
				{
					printf("child thread (%d) exit...\n", getpid());
					break;
				}
			}
			//5. quit
			close(sock_new_fd);
			close(sock_fd);
			exit(1);
		}
		else
		{
			/*------------------- main process part --------------------*/
#if 0
			//6. communication
			buff_size = sizeof(buff)/sizeof(char);

			bzero(buff, buff_size);
			do
			{
				read_ret = read(sock_new_fd, buff, (buff_size -1));
			}while((read_ret < 0) && (EINTR == errno));
			//check quit flage
			if(!strncasecmp(buff, QUIT_STR, strlen(QUIT_STR)))
				break;
			if(read_ret < 0)
			{
				perror("read");
				continue;
			}
			printf("received message: %s", buff);
#endif
			//waitpid(fork_pid, NULL, WUNTRACED);

			sleep(2);
			printf("main process [%d]", getpid());
			if(!strncasecmp(buff, QUIT_STR, strlen(QUIT_STR)))
				break;
		}
	}
	//5. quit
	close(sock_new_fd);
	close(sock_fd);

	return 0;
}







