#ifndef __IT6151_GLOBAL_H
#define __IT6151_GLOBAL_H


#define DEBUG_FLAG		1
#define TAG				"<IT6151> "
#define LEVEL			KERN_INFO

#define usr_msg(fmt, args...)						\
	do {											\
		if(DEBUG_FLAG) {							\
			printk(KERN_INFO TAG fmt"\n", ##args);	\
		}											\
	}while(0)

#define HAS_IT6151				1

#define IT6151_EDP_SLAVE_ADDR_WRITE						(0x5C << 0)
#define	IT6151_MIPIRX_SLAVE_ADDR_WRITE 					(0x6C << 0)


struct it6151_info {
	int gpio_init;
	int gpio_stdby_pwr;
	int irq_num;
	struct i2c_client * it6151_client;
};


static int it6151_i2c_init_flag;


#endif
