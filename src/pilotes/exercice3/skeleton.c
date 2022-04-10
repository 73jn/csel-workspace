/* skeleton.c */
#include <linux/cdev.h>        /* needed for char device driver */
#include <linux/fs.h>          /* needed for device drivers */
#include <linux/init.h>        /* needed for macros */
#include <linux/kernel.h>      /* needed for debugging */
#include <linux/module.h>      /* needed by all modules */
#include <linux/moduleparam.h> /* needed for module parameters */
#include <linux/slab.h>        /* needed for dynamic memory management */
#include <linux/uaccess.h>     /* needed to copy data to/from user */

#define BUFFER_SZ 10000

static int instances = 3;
module_param(instances, int, 0);

static char** buffers = 0;
static dev_t skeleton_dev;
static struct cdev skeleton_cdev;

static int skeleton_open(struct inode* i, struct file* f)
{
    printk("OPEN");
    f->private_data = buffers[iminor(i)];
    return(0);
}

static int skeleton_release(struct inode* i, struct file* f)
{
    return(0);
}

static ssize_t skeleton_read(struct file* f,
                             char __user* buf,
                             size_t count,
                             loff_t* off)
{
    printk("READ");
    ssize_t remaining = BUFFER_SZ - (ssize_t)(*off);
    if (count > remaining) count = remaining;
    *off += count; 
    char* ptr = (char*)f->private_data + *off;
    copy_to_user(buf, ptr, count);
    return(count);
}

static ssize_t skeleton_write(struct file* f,
                              const char __user* buf,
                              size_t count,
                              loff_t* off)
{
    printk("WRITE\n");
    ssize_t remaining = BUFFER_SZ - (ssize_t)(*off);
    char* ptr = (char*)f->private_data + *off;
    *off += count;
    ptr[count] = 0;
    copy_from_user(ptr, buf, count);
    return count;
}

static struct file_operations skeleton_fops = {
    .owner   = THIS_MODULE,
    .open    = skeleton_open,
    .read    = skeleton_read,
    .write   = skeleton_write,
    .release = skeleton_release,
};

static int __init skeleton_init(void)
{
    printk("INIT\n");
    //Add nInstances
    alloc_chrdev_region(&skeleton_dev, 0, instances, "mymodule");
    cdev_init(&skeleton_cdev, &skeleton_fops);
    skeleton_cdev.owner = THIS_MODULE;
    //Add nInstances
    cdev_add(&skeleton_cdev, skeleton_dev, instances);
    //Create nInstances buffer
    buffers = kzalloc(sizeof(char*) * instances, GFP_KERNEL);
    int i = 0;
    for (i = 0; i < instances; i++)
        buffers[i] = kzalloc(BUFFER_SZ, GFP_KERNEL);
    return 0;
}

static void __exit skeleton_exit(void)
{
    printk("EXIT\n");
    cdev_del(&skeleton_cdev);
    unregister_chrdev_region(skeleton_dev, instances);
    int i;
    for (i = 0; i < instances; i++) kfree(buffers[i]);
    kfree(buffers);
}

module_init(skeleton_init);
module_exit(skeleton_exit);

MODULE_DESCRIPTION("Module skeleton");
MODULE_LICENSE("GPL");
