/**
	laser_led {
		compatible = "laser_led";
		laser = <&gpio5, RK_PC3, GPIO_ACTIVE_LOW>;
		label = "laser_pin";
		pinctrl-name = "default";
		pinctrl-0 = <&laser_blink>;
		status = "okay";
	};
    &pinctrl {

        laser_blink: laser-blink {

                laser-on = <RK_GPIO5 RK_PC3 GPIO_ACTIVE_HIGH, &pcfg_pull_down>;
                laser-off = <RK_GPIO5 RK_PC3 GPIO_ACTIVE_LOW, &pcfg_pull_down>;
        }

        ......
    }


 */
#ifndef __COMMON_H


#include <linux/fs.h>

#define MAGIC_NO            'K'
#define SET 				0x01
#define RESET				0x02
#define BLINK				0x03
#define STATUS				0x04
#define TEST				0x05

#define GPIO_TEST 			_IO(MAGIC_NO, TEST)
#define GPIO_SET 			_IOW(MAGIC_NO, SET, unsigned int)
#define GPIO_RESET			_IOW(MAGIC_NO, RESET, unsigned int)
#define GPIO_BLINK			_IOW(MAGIC_NO, BLINK, unsigned int)			// param is timer delay
#define GPIO_STATUS			_IOR(MAGIC_NO, STATUS, int)


#endif
/**--------------------------------------- END LINE -------------------------------------*/