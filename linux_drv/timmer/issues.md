# kernel 4.x can not use this

/home/quan/samba_share/git_code/Learning/linux_drv/timmer/usr_timer.c:91:5: error: implicit declaration of function ‘init_timer’; did you mean ‘init_timers’? [-Werror=implicit-function-dec
laration]
     init_timer(&dev_timer->s_timer);
          ^~~~~~~~~~
	       init_timers
	       /home/quan/samba_share/git_code/Learning/linux_drv/timmer/usr_timer.c:93:33: error: assignment from incompatible pointer type [-Werror=incompatible-pointer-types]
	            dev_timer->s_timer.function = &sencod_timer_handler;
		    cc1: some warnings being treated as errors
