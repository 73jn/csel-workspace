#include <linux/module.h> /* needed by all modules */
#include <linux/init.h> /* needed for macros */ 
#include <linux/kernel.h> /* needed for debugging */
#include <linux/moduleparam.h> /* needed for module parameters */

#include <linux/platform_device.h> /* needed for sysfs handling */
#include <linux/uaccess.h>         /* needed to copy data to/from user */

#include <linux/timer.h> /* needed for timer */

#include <linux/thermal.h> /* needed for temperature */

#include <linux/gpio.h> /* needed for gpio */

// led 
#define LED 10
int k = 0; //led value

// timer
static struct timer_list timer_led;
static struct timer_list timer_temperature;
static struct thermal_zone_device* cpu_thermal_zone;

// set frequency
static int frequency = 2;
static int auto_mode = 1;
static int temperature = 10;

ssize_t sysfs_show_frequency(struct device* dev,
                       struct device_attribute* attr,
                       char* buf)
{
    sprintf(buf, "%d\n", frequency);
    return strlen(buf);
}
ssize_t sysfs_store_frequency(struct device* dev,
                        struct device_attribute* attr,
                        const char* buf,
                        size_t count)
{
    frequency = simple_strtol(buf, 0, 10);
    return count;
}
DEVICE_ATTR(frequency, 0664, sysfs_show_frequency, sysfs_store_frequency);

ssize_t sysfs_show_auto_mode(struct device* dev,
                       struct device_attribute* attr,
                       char* buf)
{
    sprintf(buf, "%d\n", auto_mode);
    return strlen(buf);
}
ssize_t sysfs_store_auto_mode(struct device* dev,
                        struct device_attribute* attr,
                        const char* buf,
                        size_t count)
{
    auto_mode = simple_strtol(buf, 0, 10);
	if (auto_mode > 0){
		auto_mode = 1;
	}
    return count;
}
DEVICE_ATTR(auto_mode, 0664, sysfs_show_auto_mode, sysfs_store_auto_mode);

ssize_t sysfs_show_temperature(struct device* dev,
                       struct device_attribute* attr,
                       char* buf)
{
    sprintf(buf, "%d\n", temperature);
    return strlen(buf);
}
ssize_t sysfs_store_temperature(struct device* dev,
                        struct device_attribute* attr,
                        const char* buf,
                        size_t count)
{
	// do nothing
    return count;
}
DEVICE_ATTR(temperature, 0664, sysfs_show_temperature, sysfs_store_temperature);

// timer callback function
static void led_timer_callback(struct timer_list *timer)
{
	// led frequency computing if in auto_mode
	if (auto_mode){
		if(temperature < 35000){	// plus petit que 35 degrès
			frequency = 2;
		}
		else if(temperature < 40000){
			frequency = 5;
		}
		else if(temperature < 45000){
			frequency = 10;				
		}
		else {
			frequency = 20;			
		}
	}


	// led toggle
	k  = (k + 1) % 2;

	gpio_set_value(LED, k);

	//printk("frequency: %d\n", frequency);

	mod_timer(&timer_led, jiffies + msecs_to_jiffies(500/frequency));
}

static void temperature_timer_callback(struct timer_list *timer)
{
	thermal_zone_get_temp(cpu_thermal_zone, &temperature);

	printk("Temperature: %d, Frequency : %d, Auto_mode : %d\n", temperature, frequency, auto_mode);

	mod_timer(&timer_temperature, jiffies + msecs_to_jiffies(2000));
}

static struct class* sysfs_class;
static struct device* sysfs_device;

static int __init skeleton_init(void)
{
	int ret;
	int status = 0;
	
	pr_info("%s: Miniproject module loaded\n", __func__);

	//pr_info ("frequency: %d\n", frequency);

	//sysfs

    sysfs_class  = class_create(THIS_MODULE, "miniproject");
    sysfs_device = device_create(sysfs_class, NULL, 0, NULL, "management");
    if (status == 0) status = device_create_file(sysfs_device, &dev_attr_frequency);
	if (status == 0) status = device_create_file(sysfs_device, &dev_attr_auto_mode);
	if (status == 0) status = device_create_file(sysfs_device, &dev_attr_temperature);

	// thermal zone

	cpu_thermal_zone = thermal_zone_get_zone_by_name("cpu-thermal");
	thermal_zone_get_temp(cpu_thermal_zone, &temperature);

	// led
	// -- setup the led gpio as output
	if(gpio_request(LED, "LED") < 0) return -1;
	gpio_direction_output(LED, 0 );

	// timer

	timer_setup(&timer_led, led_timer_callback, 0);
	timer_setup(&timer_temperature, temperature_timer_callback, 0);

	// run timer
	ret = mod_timer(&timer_led, jiffies + msecs_to_jiffies(500/frequency));
	ret = mod_timer(&timer_temperature, jiffies + msecs_to_jiffies(2000));
	
	return 0; 
}
static void __exit skeleton_exit(void)
{
	int ret;
	
	// sysfs
	device_remove_file(sysfs_device, &dev_attr_frequency);
	device_remove_file(sysfs_device, &dev_attr_auto_mode);
	device_remove_file(sysfs_device, &dev_attr_temperature);
    device_destroy(sysfs_class, 0);
    class_destroy(sysfs_class);

	// timer 
	ret = del_timer(&timer_led);
	ret = del_timer(&timer_temperature);

	// led
	gpio_set_value(LED, 0);
	gpio_free(LED);
	
	pr_info("%s: Miniproject module unloaded\n", __func__);
}

module_init (skeleton_init);
module_exit (skeleton_exit);

MODULE_DESCRIPTION ("Module skeleton"); 
MODULE_LICENSE ("GPL");