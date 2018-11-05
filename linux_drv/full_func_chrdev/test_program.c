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
#include <string.h>

#define BUFF_SIZE	1024
static const char * file_path = "/dev/device"

int main(int argv, const char **args)
{

	int ret = fopen(file_path, O_RDWR | O_RDWR)

	return 0;
}