#include "client.h"

#define BUFF_SIZE  1024

int main(int argc, char **argv)
{
    int port = SERVER_PORT; //set default port

    if (argc < 3)
    {
        printf("Usage:\n \
                \t %s server_ip  server_port\n");
        exit(1);
    }
    else if (argc == 2)
    {
        printf("default port number (%d) choose", port);
    }

    port = atoi(argv[2]);
    if (port < 5000 || port > 65535)
    {
        printf("...port out of range...\n");
        printf("5000 < port < 65535\n");
        exit(1);
    }

    //1. creat socket
    int tcp_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket_fd < 0)
    {
        perror("tcp socket creat error...");
        exit(1);
    }

    // 2. 填写服务器的 port and ip
    struct sockaddr_in addr_set;
    bzero(&addr_set, sizeof(addr_set));
    addr_set.sin_family      = AF_INET;
    addr_set.sin_port        = htons(port);
    addr_set.sin_addr.s_addr = inet_addr(argv[1]);

    //3. 连接服务器
    if (connect(tcp_socket_fd, (struct sockaddr *)&addr_set, sizeof(addr_set)) < 0)
    {
        printf("client connect to server error...\n");
        exit(1);
    }
    else
        printf("client connect established...\n");

    // 从标准键盘上输入数据
    int write_ret = -1;
    char buff[BUFF_SIZE];

    
    while(1){
        fprintf(stdout, "Please input string:");
        bzero(buff, BUFF_SIZE);
        fgets(buff, BUFF_SIZE -1, stdin);
        do{
            write_ret  = write(tcp_socket_fd, buff, strlen(buff));
        }while(write_ret < 0 && (EINTR == errno));

        if(!strncasecmp(buff, QUIT, strlen(QUIT)))   //received quit signal
            break;
    }
    close(tcp_socket_fd);
    return 0;
    




    return 0;
}