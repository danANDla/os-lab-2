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
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/dcache.h>


MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("1.0");

#define WBUFFER_SIZE 2000
#define RBUFFER_SIZE 2000

int writelen;
struct dentry* direction_entry;

static ssize_t _proc_read(struct file *f, char __user *buffer, size_t len, loff_t *offset){
    char data_rbuffer[RBUFFER_SIZE] = {0};
    char isOk;
    isOk = 0;

    loff_t msg_len = 0;
    if (*offset > 0) return 0;

    if(direction_entry != NULL){
        if(direction_entry->d_inode != NULL){
            /*
            msg_len += sprintf(data_rbuffer + msg_len, "----DENTRY INFO----\n");
            msg_len += sprintf(data_rbuffer + msg_len, "d_iname: %s\n", direction_entry->d_iname);
            msg_len += sprintf(data_rbuffer + msg_len, "inode number: %lu\n", direction_entry->d_inode->i_ino);
            msg_len += sprintf(data_rbuffer + msg_len, "inode file size: %lld\n", direction_entry->d_inode->i_size);
            msg_len += sprintf(data_rbuffer + msg_len, "dflags: %lu\n", direction_entry->d_flags);
            msg_len += sprintf(data_rbuffer + msg_len, "d_time: %lu\n", direction_entry->d_time);
            msg_len += sprintf(data_rbuffer + msg_len, "superblock size: %lu\n", direction_entry->d_sb->s_blocksize);
            msg_len += sprintf(data_rbuffer + msg_len, "-------------\n"); 
            */
            /*
            struct user_dentry{
                unsigned char * d_iname;
            };
            struct user_dentry u_dentry;
            u_dentry = (struct user_dentry){
                .d_iname = direction_entry->d_iname
            };
            memcpy(data_rbuffer, &u_dentry, sizeof(u_dentry));
            msg_len += sizeof(u_dentry);
            */
            struct user_dentry{
                //unsigned char d_iname[DNAME_INLINE_LEN]
                unsigned int d_flags;   
            };
            struct user_dentry u_dentry;
            u_dentry = (struct user_dentry){
                .d_flags = direction_entry->d_flags
            //    .d_iname = direction_entry->d_iname
            };
            isOk = 1;
            memcpy(data_rbuffer, &isOk, sizeof(char));
            msg_len += sizeof(char);
            memcpy(data_rbuffer + msg_len, &u_dentry, sizeof(struct user_dentry));
            msg_len += sizeof(u_dentry);
            pr_info("dentry wrote.");
        } else{
            pr_info("inode wasn't written.");
        }
    } else{
        msg_len = 0;
        msg_len += sprintf(data_rbuffer + msg_len, "can't acces structure\n");
    }
    return simple_read_from_buffer(buffer, msg_len, offset, data_rbuffer, RBUFFER_SIZE);
}

static ssize_t _proc_write(struct file *f, const char __user *buffer, size_t len, loff_t *offset){
    ssize_t ret;
    struct path p;
    int err;

    char data_wbuffer[WBUFFER_SIZE] = {0};
    //memset(data_wbuffer, 0, sizeof(WBUFFER_SIZE));
    ret = simple_write_to_buffer(data_wbuffer, WBUFFER_SIZE, offset, buffer, len);
    if (ret > 0){
        // writelen = max_t(int, writelen, *offset);
        writelen = *offset;
        printk(KERN_INFO " writeLen: %d\n", writelen); 

        // err = kern_path("/home/adi/ak", LOOKUP_FOLLOW, &p);
        err = kern_path(data_wbuffer, LOOKUP_FOLLOW, &p);
        if(err != 0) {
            printk(KERN_ALERT "invalid path: %s, err: %d\n", data_wbuffer, err); 
            direction_entry = NULL;
            return err;
        }
        direction_entry = p.dentry;
        printk(KERN_INFO "valid path: %s\n", data_wbuffer); 
    }

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
    writelen = 0;
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


