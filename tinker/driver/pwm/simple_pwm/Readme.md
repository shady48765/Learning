# ~~Tinker Board~~ PWM

[TOC]

##  GPIO Definition

|  PWM2   |  PWM3   |
| :-----: | :-----: |
| GPIO7C6 | GPIO7C7 |



## DTS part

```DTS
/* file rk3288-miniarm.dts -----------------------------*/
&pwm2 {
	status = "okay";
};

&pwm3 {
	status = "okay";
};

// has defined
/* file rk3288.dtsi -----------------------------*/
	/* pwm configuration -----------------------*/
	pwm2: pwm@ff680020 {
		compatible = "rockchip,rk3288-pwm";
		reg = <0x0 0xff680020 0x0 0x10>;
		#pwm-cells = <3>;
		pinctrl-names = "active";
		pinctrl-0 = <&pwm2_pin>;
		clocks = <&cru PCLK_PWM>;
		clock-names = "pwm";
		status = "disabled";
	};

	pwm3: pwm@ff680030 {
		compatible = "rockchip,rk3288-pwm";
		reg = <0x0 0xff680030 0x0 0x10>;
		#pwm-cells = <2>;
		pinctrl-names = "active";
		pinctrl-0 = <&pwm3_pin>;
		clocks = <&cru PCLK_PWM>;
		clock-names = "pwm";
		status = "disabled";
	};
	/* pwm pin definition -----------------------*/
		pwm2 {
			pwm2_pin: pwm2-pin {
				rockchip,pins = <7 22 3 &pcfg_pull_none>;
			};

			pwm2_pin_pull_down: pwm2-pin-pull-down {
				rockchip,pins = <7 22 3 &pcfg_pull_down>;
			};
		};

		pwm3 {
			pwm3_pin: pwm3-pin {
				rockchip,pins = <7 23 3 &pcfg_pull_none>;
			};

			pwm3_pin_pull_down: pwm3-pin-pull-down {
				rockchip,pins = <7 23 3 &pcfg_pull_down>;
			};
		};
```

