# make DTB to DTS
## .dtb to .dts tool
### tool path
> `/<kernel folder>/scripts/dtc`
### tool name
> `dtc`
## command make  dtb to dts 
- `./<dtc> -I dtb -O dts <source dtb file path> <target to store build dts file>`
- ** Example **
> `/arm/tinker/debian_kernel/scripts/dtc# ./dtc -I dtb -O dts ../../arch/arm/boot/dts/rk3288-miniarm.dtb -o rk3288-mini.dts`
