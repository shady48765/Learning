// define
#define TIMER_TAG                   " <TIMER> "
#define USR_MSG_LEVEL               KERN_ERR
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TIMER_TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args);
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TIMER_TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args);

// usage
usr_msg( "Setup timer to fire in 500ms (%ld)", jiffies); 
err_msg("set mod_timer failed, current jiffies = %ld\n", jiffies);
usr_msg( "out put no paramter"); 
