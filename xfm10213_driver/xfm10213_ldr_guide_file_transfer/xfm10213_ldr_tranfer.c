#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv){
    int fd;
    int index, counter = 0;
    unsigned char ldr_buff[4096] = {'\0'};
    unsigned char buff[1];
    if(2 != argc) {
        printf("----------------------------------------------------\n");
        printf("input format error:\n");
        printf("\t format: ./<app_name> <txt file path>\n");
        printf("----------------------------------------------------\n");
        return -1;
    }
    printf("transfered in: argv[0] = %s, argv[1] = %s\n", argv[0], argv[1]);
    printf("---------------------- start ------------------\n");
    fd = open(argv[1], O_RDONLY);
    if(fd < 0) {
        printf("cannot open file: %s\n", argv[1]);
        return -1;
    }

    while(read(fd, buff, 1) > 0) {
        ldr_buff[index] = buff[0];
        memset(buff, '\0', 1);

        index++;
    }
    close(fd);
    printf("----------------------------------------------------\n");
    while('\0' != ldr_buff[counter]) {
        printf("0x%x, ", ldr_buff[counter]);
        counter++;
        if(10 == counter)
            printf("\n");
    }
    printf("----------------------------------------------------\n");
    return 0;
}
