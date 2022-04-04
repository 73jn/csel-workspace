// skeleton.c
#include <linux/module.h>	// needed by all modules
#include <linux/init.h>		// needed for macros
#include <linux/kernel.h>	// needed for debugging
#include <linux/moduleparam.h>	// needed for module parameters

#include <linux/ioport.h>	/* needed for memory region handling */
#include <linux/io.h>		/* needed for mmio handling */


static int __init skeleton_init(void)
{
    printk("Enter in module\n");
    uint32_t *chipid[4];
    chipid[0] = ioremap (0x01c14000, 0x1000);
    printk("Value : %d, \n",chipid);
	printk("chipid=%08x'%08x'%08x'%08x\n",
	    chipid[0], chipid[1], chipid[2], chipid[3]);
    
    uint32_t *macaddr[2];
    macaddr[0] = ioremap(0x01c30050, 0x1000);
    macaddr[1] = ioremap(0x01c30054, 0x1000);
	printk("mac-addr=%02x:%02x:%02x:%02x:%02x:%02x\n",
			(*macaddr[1]>> 0) & 0xff,
			(*macaddr[1]>> 8) & 0xff,
			(*macaddr[1]>>16) & 0xff,
			(*macaddr[1]>>24) & 0xff,
			(*macaddr[0]>> 0) & 0xff,
			(*macaddr[0]>> 8) & 0xff);
	return 0;
}

static void __exit skeleton_exit(void)
{
	printk("Exit module\n");
}

module_init (skeleton_init);
module_exit (skeleton_exit);
MODULE_LICENSE("GPL");