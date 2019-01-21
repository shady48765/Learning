#ifndef __COMMON_CMD_H
#define __COMMON_CMD_H

#include <linux/fs.h>

#define MAG   'G'

#define CHECK			0x61
#define READ			0x62
#define WRITE			0x63

#define I2C_CHECK		_IO(MAG, CHECK)
#define I2C_WRITE		_IOW(MAG, WRITE, unsigned int)
#define I2C_READ		_IOR(MAG, READ, unsigned int)


#endif	