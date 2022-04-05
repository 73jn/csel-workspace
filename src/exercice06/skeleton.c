/* skeleton.c */
#include <linux/module.h>	/* needed by all modules */
#include <linux/init.h>		/* needed for macros */
#include <linux/kernel.h>	/* needed for debugging */

#include <linux/moduleparam.h>	/* needed for module parameters */

#include <linux/slab.h>		/* needed for dynamic memory allocation */
#include <linux/list.h>		/* needed for linked list processing */
#include <linux/string.h>	/* needed for string handling */

#include <linux/ioport.h>	/* needed for memory region handling */
#include <linux/io.h>		/* needed for mmio handling */

#include <linux/kthread.h>	/* needed for kernel thread management */
#include <linux/delay.h>	/* needed for delay fonctions */

static struct task_struct* thread;

static int mythread(void* data){
    printk("Start the thread\n");
    while (!kthread_should_stop()) {
        /* do something... */
        ssleep(5);
        printk("Inside the thread");
    }
    printk("Leave the thread");
    return(0);
}
static int __init skeleton_init(void)
{
    printk("Enter, launch the thread");
    thread = kthread_run(mythread, NULL, "thread name" );
	return 0;
}

static void __exit skeleton_exit(void)
{
	printk("Exit module");
    kthread_stop(thread);
}

module_init (skeleton_init);
module_exit (skeleton_exit);
MODULE_LICENSE("GPL");