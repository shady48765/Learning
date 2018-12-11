/**
 * @Description : file for linux driver test.
 * @Test function: open, read, write, close, ioctl and o on.
 * @Author :
 * @Date :
 * @Version :
 * ----------------------------------------------------------------------
 * @ioctl param description:
 * @CMD:
 * @Magic number :
 *
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "./common.h"

#define HIGH        1U
#define LOW         0U

struct pin_info {
    unsigned int on;
    unsigned int off;
    int value;
}

int main(int argc, char *argv[])
{
    int ret;
    int fd;
    int counter = 0;

    struct pin_info laser_info;
    laser_info.on = HIGH;
    laser_info.off = LOW;
    laser_info.value = -1;

    printf("---> open /dev/laser, permisson read and write\n");
    fd = open("/dev/laser", O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    printf("---> open success. ready to send data:\n");

    while (counter < 10) {
        ioctl(fd, GPIO_TEST, 0);
        ioctl(fd, GPIO_SET, &laser_info.on);
        printf("<app>--- set on = %d\n", laser_info.on);
        ioctl(fd, GPIO_RESET, &laser_info.off);
        printf("<app>--- set off = %d\n", laser_info.off);
        ioctl(fd, GPIO_STATUS, &laser_info.value);
        printf("<app>--- gpio status = %d\n", laser_info.value);
    }

    printf("--> close fd\n");
    close(fd);
    return 0;
}
