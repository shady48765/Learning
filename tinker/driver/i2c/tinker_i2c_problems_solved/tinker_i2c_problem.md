# i2c driver and solve

## i2c probe

### probe problem in DTS

- using DTS

如果使用 DTS 方式仍然需要配置 id_table 不然会匹配不上i2c


```C
static const struct of_device_id oled_i2c_of_match[] = {
	{.compatible = "oled,ssd1306"},
	{/* keep this */}
};
MODULE_DEVICE_TABLE(of, oled_i2c_of_match);

static const struct i2c_device_id oled_device_id[] = {
    {"oled,ssd1306", 0},
    {/*keep this*/}
};
MODULE_DEVICE_TABLE(i2c, oled_device_id);


static struct i2c_driver oled_i2c_driver = {
    .driver = {
        .owner = THIS_MODULE,
        .name = OLED_DEV_NAME,
        .of_match_table = of_match_ptr(oled_i2c_of_match),
    },
    .probe = oled_i2c_probe,
    .remove = oled_i2c_remove,
    .id_table = oled_device_id,
}; 


/*-----------------------------------------------------------------*/
// i2c check
#if I2C_CHECK_SELF
    usr_msg("---> move in");
    if(!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err_msg("i2c check failed");
		return -ENODEV;
    }
#endif
```

