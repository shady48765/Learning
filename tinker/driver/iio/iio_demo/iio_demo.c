#include "iio_demo.h"


struct iio_dev	dev;


#define DEMO_CHANNEL(channel_index)   			\
	{                               			\
	    .type = IIO_VOLTAGE,        			\
	    .indexed = 1,               			\
	    .channel = (channel_index),           	\
	    .address = (channel_index),          	\
	    .info_mask_separate = BIT(IIO_CHAN_INFO_RAW),           \
	    .info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE)    \
	}

struct private_data {
	int foo;
	int bar;
	struct mutex lock;
};

static const struct iio_chan_spec channls [] = {
	DEMO_CHANNEL(0),
	DEMO_CHANNEL(1),
	DEMO_CHANNEL(2),
};

static int demo_read(struct iio_dev *indio_dev, struct iio_chan_spec const *chan,
						int *val, int *val2, long mask)
{
	usr_msg("%s", __func__);

	return 0;
}

static int demo_write(struct iio_dev *indio_dev, struct iio_chan_spec const *chan,
						int val, int val2, long mask)
{
	usr_msg("%s", __func__);

	return 0;
}


static const struct of_device_id iio_dummy_ids[] = {
    { .compatible = "packt,iio-dummy-random", },
    { /* sentinel */ }
};

static const struct iio_info demo_iio_info = {
	.read_raw = demo_read,
	.write_raw = demo_write,
	.driver_module = THIS_MODULE,
};


static int demo_probe(struct platform_device *pdev)
{
	struct iio_dev *indio_dev;
	struct private_data *data;

	indio_dev = devm_iio_device_alloc(&pdev->dev, sizeof(*data));
	if (!indio_dev) {
		err_msg("iio allocation failed!");
		return -ENOMEM;
	}

	data = iio_priv(indio_dev);
	mutex_init(&data->lock);
	indio_dev->dev.parent           = &pdev->dev;
	indio_dev->info                 = &demo_iio_info;
	indio_dev->name                 = KBUILD_MODNAME;	// KBUILD_MODNAME, macro defined for module name
	indio_dev->modes                = INDIO_DIRECT_MODE;
	indio_dev->channels             = channls;
	indio_dev->num_channels         = ARRAY_SIZE(channls);
	indio_dev->available_scan_masks = 0xF;
	iio_device_register(indio_dev);

	platform_set_drvdata(pdev, indio_dev);
	return 0;
}

static int demo_remove(struct platform_device *pdev)
{
	struct iio_dev *indio_dev = platform_get_drvdata(pdev);
	iio_device_unregister(indio_dev);
	return 0;
}

static struct platform_driver mypdrv = {
	.probe      = demo_probe,
	.remove     = demo_remove,
	.driver     = {
		.name           = "iio-dummy-random",
		.of_match_table = of_match_ptr(iio_dummy_ids),  
		.owner          = THIS_MODULE,
	},
};



static int __init iio_demo_init(void)
{
    usr_msg("---> moved in %s ", __func__);
	platform_driver_register(&mypdrv);
	return 0;
}

static void __exit iio_demo_exit(void)
{
    usr_msg("---> moved in %s ", __func__);
 	platform_driver_unregister(&mypdrv);
}


module_init(iio_demo_init);
module_exit(iio_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("V");
MODULE_DESCRIPTION("iio demo.");
