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
#include <linux/wait.h>


static struct task_struct* pthread1;
static struct task_struct* pthread2;
DECLARE_WAIT_QUEUE_HEAD(queue);
static atomic_t is_kicked;
static int thread2(void* data){
    printk("Start the thread 2\n");
    while (!kthread_should_stop()) {
        printk("Inside the thread 2");
        printk("Atomic read thread 2 : %d", atomic_read(&is_kicked));
        ssleep(5);
        printk("Thread 2 : check dodo");
        int status = wait_event_interruptible (queue, atomic_read(&is_kicked) == 1 || kthread_should_stop());
		if (status == -ERESTARTSYS) {
			printk("thread 2 has been interrupted\n");
			break;
        }
		atomic_set (&is_kicked, 0);
		pr_info ("skeleton thread 2 has been kicked\n");
    }
    printk("Leave the thread 2");
    return(0);
}
static int thread1(void* data){
    printk("Start the thread 1\n");
    ssleep(1);
    atomic_set(&is_kicked, 1); //DODO
    
    while (!kthread_should_stop()) {
		ssleep(5);
		printk("skeleton thread 1 timout elapsed...\n");
		atomic_set (&is_kicked, 1);
		wake_up_interruptible (&queue);
    }
    printk("Leave the thread 1");
    return(0);
}
static int __init skeleton_init(void)
{
    printk("Enter, launch the thread");
    pthread1 = kthread_run(thread1, NULL, "thread 1");
    pthread2 = kthread_run(thread2, NULL, "thread 2");
	return 0;
}

static void __exit skeleton_exit(void)
{
	printk("Exit module");
    //kthread_stop(pthread2);
    //kthread_stop(pthread1);
}

module_init (skeleton_init);
module_exit (skeleton_exit);
MODULE_LICENSE("GPL");