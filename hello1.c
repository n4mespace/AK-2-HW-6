// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <hello1.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Zakharchuk IV-82\n");
MODULE_DESCRIPTION("AK-2 lab_6 (advanced task): hello1.c\n");

struct timeit_list {
	struct list_head node;
	ktime_t before;
	ktime_t after;
};
LIST_HEAD(head_node);


int print_hello(uint n)
{
	struct timeit_list *list;
	uint i;

	if (n < 0) {
		pr_err("ERROR! n < 0\n");
	} else if (n == 0) {
		pr_warn("WARNING! n = 0\n");
	} else if (n >= 5 && n <= 10) {
		pr_warn("WARNING! 5 <= n <= 10\n");
	} else if (n > 10) {
		pr_err("ERROR! n > 10\n");
		return -EINVAL;
	}

	for (i = 0; i < n; i++) {
		list = kmalloc(sizeof(struct timeit_list), GFP_KERNEL);
		list->before = ktime_get();
		pr_info("Hello, world!\n");
		list->after = ktime_get();
		INIT_LIST_HEAD(&list->node);
		list_add_tail(&list->node, &head_node);
	}
	return 0;
}
EXPORT_SYMBOL(print_hello);


static int __init init_hello(void)
{
	pr_info("hello1 init\n");
	return 0;
}


static void __exit exit_hello(void)
{
	struct timeit_list *list, *tmp;

	list_for_each_entry_safe(list, tmp, &head_node, node) {
		pr_info("Time: %lld", list->after - list->before);
		list_del(&list->node);
		kfree(list);
	}
	BUG_ON(!list_empty(&head_node));
	pr_info("hello1 exit\n");
}


module_init(init_hello);
module_exit(exit_hello);
