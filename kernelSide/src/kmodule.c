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

//struct proc_data {
//    int readlen;
//    char *rbuffer;
//    int writelen;
//    int maxwritelen;
//    char *wbuffer;
//};

static char data_wbuffer[WBUFFER_SIZE];
static char data_rbuffer[RBUFFER_SIZE];
int writelen;

static ssize_t _proc_read(struct file *f, char __user *buffer, size_t len, loff_t *offset){
//    if(count>temp){
//        count=temp;
//    }
//    temp=temp-count;
//    copy_to_user(buf,data_buf, count);
//    if(count==0)
//        temp=buff_len;
//    return count;
//
//    loff_t msg_len = 0;
//    if (!module_data.rbuffer) return -EINVAL;
//    msg_len += sprintf(module_data.rbuffer + msg_len, "hello from kernel\n");
//    return simple_read_from_buffer(buffer, msg_len, offset, module_data.rbuffer, module_data.readlen);

    loff_t msg_len = 0;

    msg_len += sprintf(data_rbuffer + msg_len, "hello from kernel\n");
    return simple_read_from_buffer(buffer, msg_len, offset, data_rbuffer, RBUFFER_SIZE);
}

static ssize_t _proc_write(struct file *f, const char __user *buffer, size_t len, loff_t *offset){
//    memset(data_buf, '\0', strbuff_len(data_buf));
//    copy_from_user(data_buf,buf,count);
//    buff_len=count;
//    temp=buff_len;
//    d_point = data_buf;
//    buff_len=strbuff_len(data_buf);
//    temp=buff_len;
//    return count;
//
//    ssize_t ret;
//    if (!module_data.wbuffer) return -EINVAL;
//
//    ret = simple_write_to_buffer(module_data.wbuffer, module_data.maxwritelen, offset, buffer, len);
//    if (ret > 0) module_data.writelen = max_t(int, module_data.writelen, *offset);
//
//    return ret;    
//
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


