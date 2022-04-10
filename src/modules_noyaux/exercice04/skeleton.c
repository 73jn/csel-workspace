// skeleton.c
#include <linux/module.h>	// needed by all modules
#include <linux/init.h>		// needed for macros
#include <linux/kernel.h>	// needed for debugging
#include <linux/moduleparam.h>	// needed for module parameters
#include <linux/string.h>
#include <linux/slab.h>		/* needed for dynamic memory allocation */
#include <linux/list.h>		/* needed for linked list processing */

static char* param1 = "";
module_param(param1, charp, 0);

static int param2 = 0;
module_param(param2, int, 0);

struct element {
	char text[100];
	int ele_nr;
	struct list_head node;
};

static LIST_HEAD (my_list);

static int __init skeleton_init(void)
{
	int i = 0;
    printk("Enter in module, param1 = %s, param2 = %d", param1, param2);
	for (i = 0; i < param2; i++) {
		struct element* ele = kzalloc (sizeof(*ele), GFP_KERNEL);
		if (ele != 0) {
			strncpy (ele->text, param1, 99);
			ele->ele_nr = i;
			list_add_tail(&ele->node, &my_list);
		}
	}
	return 0;
}

static void __exit skeleton_exit(void)
{
	printk("Exit module");
	struct element* ele;
	int nb_eles = 0;
	list_for_each_entry (ele, &my_list, node) {
		nb_eles++;
	}
	while (!list_empty (&my_list)) {
		ele = list_entry (my_list.next, struct element, node);
		list_del (&ele->node);
		kfree (ele);
	}
}

module_init (skeleton_init);
module_exit (skeleton_exit);
MODULE_LICENSE("GPL");