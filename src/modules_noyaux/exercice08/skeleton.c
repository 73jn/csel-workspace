/* skeleton.c */
#include <linux/module.h>	/* needed by all modules */
#include <linux/init.h>		/* needed for macros */
#include <linux/kernel.h>	/* needed for debugging */

#include <linux/interrupt.h>	/* needed for interrupt handling */
#include <linux/gpio.h>			/* needed for i/o handling */


#define K1	0
#define K2	2
#define K3	3

static char* k1="gpio_a.0-k1";
static char* k2="gpio_a.2-k2";
static char* k3="gpio_a.3-k3";


irqreturn_t short_interrupt(int irq, void* dev_id)
{
	printk("Interrupt %s happen\n", (char*)dev_id);
	return IRQ_HANDLED;
}


static int __init skeleton_init(void)
{
    gpio_request (K1, "k1");
	request_irq(gpio_to_irq(K1), short_interrupt, IRQF_TRIGGER_FALLING | IRQF_SHARED, k1, k1);
    return (0);
}

static void __exit skeleton_exit(void)
{
    gpio_free(K1);
	free_irq(gpio_to_irq(K1), k1);
}

module_init (skeleton_init);
module_exit (skeleton_exit);

MODULE_AUTHOR ("Daniel Gachet <daniel.gachet@hefr.ch>");
MODULE_DESCRIPTION ("Module skeleton");
MODULE_LICENSE ("GPL");
