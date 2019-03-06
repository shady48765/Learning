- 编译
	`make`
- 清除
	`make cleanall`
- 安装驱动
	`make install`
- 移除驱动
	`make remove`

- sysfs 路径
	`/sys/foo_sysfs/`
- 使用
	- 修改定时器计数值
	`echo <millisecond> > change_ticks`
	- 读取定时器循环次数
	`cat read_loops`
- log查看工具使用
	1. `cd /root/tools`
	2. `./watch_log.sh`