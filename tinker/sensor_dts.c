/* dts definition start -----------------------------------------------------------------------------------------*/
/* touch panel */
&touch {
	tpd-resolution = <800 1280>;
	use-tpd-button = <0>;
	tpd-key-num = <3>;
	tpd-key-local= <139 172 158 0>;
	tpd-key-dim-local = <90 883 	100 40 230 883 100 40 370 883 100 40 0 0 0 0>;
	tpd-max-touch-num = <5>;
	tpd-filter-enable = <1>;
	tpd-filter-pixel-density = <93>;
	tpd-filter-custom-prameters = <0 0 0 0 0 0 0 0 0 0 0 0>;
	tpd-filter-custom-speed = <0 0 0>;
	pinctrl-names = "default", "state_eint_as_int", "state_eint_output0", "state_eint_output1",
		"state_rst_output0", "state_rst_output1";
	pinctrl-0 = <&CTP_pins_default>;
	pinctrl-1 = <&CTP_pins_eint_as_int>;
	pinctrl-2 = <&CTP_pins_eint_output0>;
	pinctrl-3 = <&CTP_pins_eint_output1>;
	pinctrl-4 = <&CTP_pins_rst_output0>;
	pinctrl-5 = <&CTP_pins_rst_output1>;
	status = "okay";
};
&pio {
	CTP_pins_default: eint0default {
	};

	accdet_pins_default: eint6default {
	};

	accdet_pins_eint_as_int: eint6 {
		pins_cmd_dat {
			pins = <PINMUX_GPIO6__FUNC_GPIO6>;
			bias-disable;
		};
	};

	CTP_pins_eint_as_int: eint@0 {
		pins_cmd_dat {
			pins = <PINMUX_GPIO10__FUNC_GPIO10>;
			slew-rate = <0>;
			bias-disable;
		};
	};
	CTP_pins_eint_output0: eintoutput0 {
		pins_cmd_dat {
			pins = <PINMUX_GPIO10__FUNC_GPIO10>;
			slew-rate = <1>;
			output-low;
		};
	};
	CTP_pins_eint_output1: eintoutput1 {
		pins_cmd_dat {
			pins = <PINMUX_GPIO10__FUNC_GPIO10>;
			slew-rate = <1>;
			output-high;
		};
	};
	CTP_pins_rst_output0: rstoutput0 {
		pins_cmd_dat {
			pins = <PINMUX_GPIO62__FUNC_GPIO62>;
			slew-rate = <1>;
			output-low;
		};
	};
	CTP_pins_rst_output1: rstoutput1 {
		pins_cmd_dat {
			pins = <PINMUX_GPIO62__FUNC_GPIO62>;
			slew-rate = <1>;
			output-high;
		};
	};
};


&i2c1 {
	silead_touch@40 {
		compatible = "mediatek,silead_touch";
		reg = <0x40>;
		
		interrupt-parent = <&eintc>;
		interrupts = <10 IRQ_TYPE_EDGE_FALLING>;
		debounce = <10 0>;
	};
};
/* TOUCH end */
/* dts definition end -----------------------------------------------------------------------------------------*/
struct pinctrl *pinctrl1;
struct pinctrl_state *pins_default;
struct pinctrl_state *eint_as_int, *eint_output0, *eint_output1, *rst_output0, *rst_output1;
struct of_device_id touch_of_match[] = {
	{ .compatible = "mediatek,mt6570-touch", },
	{ .compatible = "mediatek,mt6735-touch", },
	{ .compatible = "mediatek,mt6580-touch", },
	{ .compatible = "mediatek,mt8173-touch", },
	{ .compatible = "mediatek,mt6755-touch", },
	{ .compatible = "mediatek,mt6757-touch", },
	{ .compatible = "mediatek,mt6797-touch", },
	{ .compatible = "mediatek,mt8163-touch", },
	{ .compatible = "mediatek,mt8127-touch", },
	{ .compatible = "mediatek,mt2701-touch", },
	{ .compatible = "mediatek,mt7623-touch", },
	{},
};
EXPORT_SYMBOL(touch_of_match);

void tpd_get_dts_info(void)
{
	struct device_node *node1 = NULL;
// modified by elink_phil start <<<
#if 0
	int key_dim_local[16], i;
#endif
	// 查找匹配的 节点 node node1 = NULL 表示从根节点开始找
	node1 = of_find_matching_node(node1, touch_of_match);
	if (node1) {
		of_property_read_u32(node1, "tpd-filter-enable", &tpd_dts_data.touch_filter.enable);
		if (tpd_dts_data.touch_filter.enable) {
			of_property_read_u32(node1, "tpd-filter-pixel-density",
			                     &tpd_dts_data.touch_filter.pixel_density);
			of_property_read_u32_array(node1, "tpd-filter-custom-prameters",
			                           (u32 *)tpd_dts_data.touch_filter.W_W, ARRAY_SIZE(tpd_dts_data.touch_filter.W_W));
			of_property_read_u32_array(node1, "tpd-filter-custom-speed",
			                           tpd_dts_data.touch_filter.VECLOCITY_THRESHOLD,
			                           ARRAY_SIZE(tpd_dts_data.touch_filter.VECLOCITY_THRESHOLD));
		}
		memcpy(&tpd_filter, &tpd_dts_data.touch_filter, sizeof(tpd_filter));
		pr_debug("[tpd]tpd-filter-enable = %d, pixel_density = %d\n",
		         tpd_filter.enable, tpd_filter.pixel_density);
	} else {
		pr_err("[tpd]%s can't find touch compatible custom node\n", __func__);
	}
}
EXPORT_SYMBOL(tpd_get_dts_info);

static DEFINE_MUTEX(tpd_set_gpio_mutex);
void tpd_gpio_as_int(int pin)
{
	mutex_lock(&tpd_set_gpio_mutex);
	TPD_DEBUG("[tpd]tpd_gpio_as_int\n");
	if (pin == 1)
		pinctrl_select_state(pinctrl1, eint_as_int);
	mutex_unlock(&tpd_set_gpio_mutex);
}
EXPORT_SYMBOL(tpd_gpio_as_int);

void tpd_gpio_output(int pin, int level)
{
	mutex_lock(&tpd_set_gpio_mutex);
	TPD_DEBUG("[tpd]tpd_gpio_output pin = %d, level = %d\n", pin, level);
	if (pin == 1) {
		if (level)
			pinctrl_select_state(pinctrl1, eint_output1);
		else
			pinctrl_select_state(pinctrl1, eint_output0);
	} else {
		if (level)
			pinctrl_select_state(pinctrl1, rst_output1);
		else
			pinctrl_select_state(pinctrl1, rst_output0);
	}
	mutex_unlock(&tpd_set_gpio_mutex);
}
EXPORT_SYMBOL(tpd_gpio_output);

int tpd_get_gpio_info(struct platform_device *pdev)
{
	int ret;

	TPD_DEBUG("[tpd %d] mt_tpd_pinctrl+++++++++++++++++\n", pdev->id);
	pinctrl1 = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(pinctrl1)) {
		ret = PTR_ERR(pinctrl1);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl1!\n");
		return ret;
	}
	pins_default = pinctrl_lookup_state(pinctrl1, "default");
	if (IS_ERR(pins_default)) {
		ret = PTR_ERR(pins_default);
		/* dev_err(&pdev->dev, "fwq Cannot find touch pinctrl default %d!\n", ret);*/
	}
	eint_as_int = pinctrl_lookup_state(pinctrl1, "state_eint_as_int");
	if (IS_ERR(eint_as_int)) {
		ret = PTR_ERR(eint_as_int);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_eint_as_int!\n");
		return ret;
	}
	eint_output0 = pinctrl_lookup_state(pinctrl1, "state_eint_output0");
	if (IS_ERR(eint_output0)) {
		ret = PTR_ERR(eint_output0);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_eint_output0!\n");
		return ret;
	}
	eint_output1 = pinctrl_lookup_state(pinctrl1, "state_eint_output1");
	if (IS_ERR(eint_output1)) {
		ret = PTR_ERR(eint_output1);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_eint_output1!\n");
		return ret;
	}
	rst_output0 = pinctrl_lookup_state(pinctrl1, "state_rst_output0");
	if (IS_ERR(rst_output0)) {
		ret = PTR_ERR(rst_output0);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_rst_output0!\n");
		return ret;
	}
	rst_output1 = pinctrl_lookup_state(pinctrl1, "state_rst_output1");
	if (IS_ERR(rst_output1)) {
		ret = PTR_ERR(rst_output1);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_rst_output1!\n");
		return ret;
	}
	TPD_DEBUG("[tpd%d] mt_tpd_pinctrl----------\n", pdev->id);
	return 0;
}