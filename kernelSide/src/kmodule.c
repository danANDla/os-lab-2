#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/pid.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/string.h>


MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("1.0");

#define WBUFFER_SIZE 2000
#define RBUFFER_SIZE 2000

static char data_wbuffer[WBUFFER_SIZE];
static char data_rbuffer[RBUFFER_SIZE];
int writelen;

static ssize_t _proc_read(struct file *f, char __user *buffer, size_t len, loff_t *offset){
    loff_t msg_len = 0;

    msg_len += sprintf(data_rbuffer + msg_len, "hello from kernel\n");
    return simple_read_from_buffer(buffer, msg_len, offset, data_rbuffer, RBUFFER_SIZE);
}

static ssize_t _proc_write(struct file *f, const char __user *buffer, size_t len, loff_t *offset){
    ssize_t ret;

    ret = simple_write_to_buffer(data_wbuffer, WBUFFER_SIZE, offset, buffer, len);
    if (ret > 0) writelen = max_t(int, writelen, *offset);

    return ret;    
}

static struct proc_ops f_ops = {
    .proc_read = _proc_read,
    .proc_write = _proc_write
};

static void create_entry(void){
    proc_create("sreader", 0777, NULL, &f_ops);
}

static int __init sreader_init(void){
    printk(KERN_INFO "sreader: module loading\n");
    create_entry();
    return 0;
}

static void __exit sreader_exit(void){
    remove_proc_entry("sreader", NULL);
    printk(KERN_INFO "sreader: cleaning up module\n");
}
module_init(sreader_init);
module_exit(sreader_exit);


