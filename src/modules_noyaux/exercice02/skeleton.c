// skeleton.c
#include <linux/module.h>	// needed by all modules
#include <linux/init.h>		// needed for macros
#include <linux/kernel.h>	// needed for debugging
#include <linux/moduleparam.h>	// needed for module parameters


static int param1 = 0;
module_param(param1, int, 0);

static int param2 = 0;
module_param(param2, int, 0);

static int __init skeleton_init(void)
{
    printk("Enter in module, param1 = %d, param2 = %d", param1, param2);
	return 0;
}

static void __exit skeleton_exit(void)
{
	printk("Exit module");
}

module_init (skeleton_init);
module_exit (skeleton_exit);
MODULE_LICENSE("GPL");