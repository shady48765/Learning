#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xeadeda83, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xfa8b9545, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x85858b9d, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0xeb9c2039, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x93663f7c, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xb587ca13, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x2f784409, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xad35c3c0, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xffd6b8bd, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "754FEDF93A981CC543D58A0");
