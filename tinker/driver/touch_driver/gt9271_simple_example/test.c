
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/input.h>

typedef struct {
	struct workqueue_struct *myqueue;
	struct work_struct mywork;
	struct i2c_client *cli;	
	struct input_dev  *input;
}work_t;

#define CLR_IRQ 0x814E

int gt9271_write(struct i2c_client *cli, u16 reg_addr, u8 val)
{
	u8 data[3] = {reg_addr>>8, reg_addr&0xff, val};
	struct i2c_msg msg;

	msg.addr = cli->addr;
	msg.flags = 0;
	msg.len = 3;
	msg.buf = data;

	if (i2c_transfer(cli->adapter, &msg, 1) != 1)
	{
		printk("transfer failed\n");
		return -ENODEV;
	}
	return 0;
}


int gt9271_read(struct i2c_client *cli, u16 reg_addr, char *data, int len)
{
	struct i2c_msg wmsg, rmsg;
	u8 addrs[2] = {reg_addr>>8, reg_addr&0xff};

	wmsg.addr = cli->addr;
	wmsg.flags = 0;
	wmsg.len = 2;
	wmsg.buf = addrs;
	if (i2c_transfer(cli->adapter, &wmsg, 1) != 1)
	{
		printk("write failed\n");
		return -EBUSY;
	}

	rmsg.addr = cli->addr;
	rmsg.flags = I2C_M_RD;
	rmsg.len = len;
	rmsg.buf = data;

	if (i2c_transfer(cli->adapter, &rmsg, 1) != 1)
	{
		printk("r failed\n");
		return -EBUSY;
	}
	
	return 0;
}

void work_func(struct work_struct *work)
{
	static int flag = 0;
	work_t *wk = container_of(work, work_t, mywork);
	unsigned char data[8];
	int x, y, num;

	if (0 == gt9271_read(wk->cli, CLR_IRQ, data, sizeof(data)))
	{
		x = (data[3]<<8) | data[2];
		y = (data[5]<<8) | data[4];
		num = data[0] & 0xf;
	
		if (num >= 1)
		{
			flag = 1;
			input_report_abs(wk->input, ABS_X, x);
			input_report_abs(wk->input, ABS_Y, y);
			input_report_key(wk->input, BTN_TOUCH, 1);
			input_report_abs(wk->input, ABS_PRESSURE, 1);
			input_sync(wk->input);
		}
		else
		{
			if (flag)
			{
				flag = 0;
				input_report_key(wk->input, BTN_TOUCH, 0);
				input_report_abs(wk->input, ABS_PRESSURE, 0);
				input_sync(wk->input);
			}
		}
//		printk("%d, x = %d, y = %d\n", data[0]&0xf, x, y);
	}

	gt9271_write(wk->cli, CLR_IRQ, 0); //清除gt9271芯片内部的中断
	enable_irq(wk->cli->irq);
}

irqreturn_t ts_irq(int irqno, void *arg)
{	
	work_t *wk = (work_t *)arg;

	disable_irq_nosync(irqno);

	queue_work(wk->myqueue, &wk->mywork);	

	return IRQ_HANDLED;
}

int myprobe(struct i2c_client *cli, const struct i2c_device_id *id)
{
	int ret = -EINVAL;
	work_t *wk;
	struct input_dev *dev;

	wk = kzalloc(sizeof(*wk), GFP_KERNEL);
	printk("in myprobe ...%s, %x\n", cli->name, id->driver_data);
	
	wk->myqueue = create_singlethread_workqueue(cli->name);
	if (NULL == wk->myqueue)
		goto err0;

	INIT_WORK(&wk->mywork, work_func);
	wk->cli = cli;
	ret = request_irq(cli->irq, ts_irq, IRQF_TRIGGER_FALLING, cli->name,  wk);	
	if (ret < 0)
		goto err1;

	i2c_set_clientdata(cli, wk);	
	dev = input_allocate_device();
	dev->name = "myts";
	
	set_bit(EV_KEY, dev->evbit);
	set_bit(EV_ABS, dev->evbit);
	set_bit(BTN_TOUCH, dev->keybit);

	set_bit(ABS_X, dev->absbit);
	set_bit(ABS_Y, dev->absbit);
	set_bit(ABS_PRESSURE, dev->absbit);

	input_set_abs_params(dev, ABS_X, 0, 1980, 0, 0);	
	input_set_abs_params(dev, ABS_Y, 0, 1200, 0, 0);	
	input_set_abs_params(dev, ABS_PRESSURE, 0, 1, 0, 0);	

	if (input_register_device(dev) < 0)
			goto err2;
				
	wk->input = dev;
	return 0;
err2:
	free_irq(cli->irq, wk);
err1:
	destroy_workqueue(wk->myqueue);
err0:
	kfree(wk);
	return ret;
}

int myremove(struct i2c_client *cli)
{
	work_t *wk = i2c_get_clientdata(cli);

	printk("in myremove ...%s\n", cli->name);
	free_irq(cli->irq, wk);
	
	cancel_work_sync(&wk->mywork);
	destroy_workqueue(wk->myqueue);
	
	input_unregister_device(wk->input);
	kfree(wk);
	return 0;
}

struct i2c_device_id ids[] = {
	{"gt9271", 0x3344},
	{},
};

//i2c设备驱动不管接在哪个控制器上面的设备都会进行匹配，只要符合id_table里的名字即可
struct i2c_driver mydrv = {
	.driver = {
		.name = "mydrv",
	},
	.probe = myprobe,
	.remove = myremove,
	.id_table = ids,	
};

module_i2c_driver(mydrv);

MODULE_LICENSE("GPL");

