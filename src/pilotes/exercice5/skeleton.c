/* skeleton.c */
#include <linux/cdev.h>   /* needed for char device driver */
#include <linux/device.h> /* needed for sysfs handling */
#include <linux/fs.h>     /* needed for device drivers */
#include <linux/init.h>   /* needed for macros */
#include <linux/kernel.h> /* needed for debugging */
#include <linux/miscdevice.h>
#include <linux/module.h>          /* needed by all modules */
#include <linux/platform_device.h> /* needed for sysfs handling */
#include <linux/uaccess.h>         /* needed to copy data to/from user */

struct skeleton_config {
    int id;
    long ref;
    char name[30];
    char descr[30];
};

static struct skeleton_config config;
static int val;

ssize_t sysfs_show_val(struct device* dev,
                       struct device_attribute* attr,
                       char* buf)
{
    sprintf(buf, "%d\n", val);
    return strlen(buf);
}
ssize_t sysfs_store_val(struct device* dev,
                        struct device_attribute* attr,
                        const char* buf,
                        size_t count)
{
    val = simple_strtol(buf, 0, 10);
    return count;
}
DEVICE_ATTR(val, 0664, sysfs_show_val, sysfs_store_val);

ssize_t sysfs_show_cfg(struct device* dev,
                       struct device_attribute* attr,
                       char* buf)
{
    sprintf(buf,
            "%d %ld %s %s\n",
            config.id,
            config.ref,
            config.name,
            config.descr);
    return strlen(buf);
}
ssize_t sysfs_store_cfg(struct device* dev,
                        struct device_attribute* attr,
                        const char* buf,
                        size_t count)
{
    sscanf(buf,
           "%d %ld %s %s",
           &config.id,
           &config.ref,
           config.name,
           config.descr);
    return count;
}
DEVICE_ATTR(cfg, 0664, sysfs_show_cfg, sysfs_store_cfg);

static struct class* sysfs_class;
static struct device* sysfs_device;

static int __init skeleton_init(void)
{
    int status = 0;
    sysfs_class  = class_create(THIS_MODULE, "my_sysfs_class");
    sysfs_device = device_create(sysfs_class, NULL, 0, NULL, "my_sysfs_device");
    if (status == 0) status = device_create_file(sysfs_device, &dev_attr_val);
    if (status == 0) status = device_create_file(sysfs_device, &dev_attr_cfg);

    pr_info("Linux module skeleton loaded\n");
    return 0;
}

static void __exit skeleton_exit(void)
{
    device_remove_file(sysfs_device, &dev_attr_val);
    device_remove_file(sysfs_device, &dev_attr_cfg);
    device_destroy(sysfs_class, 0);
    class_destroy(sysfs_class);
}

module_init(skeleton_init);
module_exit(skeleton_exit);

MODULE_DESCRIPTION("Module skeleton");
MODULE_LICENSE("GPL");
