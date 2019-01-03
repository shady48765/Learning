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

/**
 * @Description : file "common.h" support the ioctl command for linux driver and linux app use
 * @Detail
 * 		MAGIC_NO: 	magic number set to 'K', need to check if it's conflict about linux kernel defined.
 * 		_IO(magic_number, personal_number, parameter)
 * 			|__ parameter canbe any type of var, or struct and so on. remeber to change type to basic var type(int, double, float).
 * 					when using change type to orignal type is ok for use.
 */

#ifndef __COMMON_H


#include <linux/fs.h>

#define MAGIC_NO            'G'
#define SET 				0x61
#define RESET				0x62
#define BLINK				0x63
#define STATUS				0x64
#define TEST				0x65

#define LED_TEST 			_IO(MAGIC_NO, TEST)
#define LED_SET 			_IOW(MAGIC_NO, SET, unsigned int)
#define LED_RESET			_IOW(MAGIC_NO, RESET, unsigned int)
#define LED_BLINK			_IOW(MAGIC_NO, BLINK, unsigned int)			// param is timer delay
#define LED_STATUS			_IOR(MAGIC_NO, STATUS, int)


#endif
/**--------------------------------------- END LINE -------------------------------------*/