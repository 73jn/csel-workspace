// skeleton.c
#include <linux/module.h>	// needed by all modules
#include <linux/init.h>		// needed for macros
#include <linux/kernel.h>	// needed for debugging


static int __init skeleton_init(void)
{
    printk("Enter");
	return 0;
}

static void __exit skeleton_exit(void)
{
	printk("Exit module");
}

module_init (skeleton_init);
module_exit (skeleton_exit);
MODULE_LICENSE("GPL");