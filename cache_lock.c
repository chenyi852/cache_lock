nclude <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>

typedef struct cacheline_lock
{
    phys_addr_t paddr;
    phys_addr_t plen;
}t_cacheline_lock;

t_cacheline_lock cacheway[8];

static ssize_t cl_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    char kbuf[256];
    char *cur;
    int i, len;

    cur = kbuf;
    for (i = 0; i < sizeof(cacheway) / sizeof (cacheway[0]); i++){
        len  = sprintf(cur, "%d:0x%llx@0x%llx\n", i, cacheway[i].paddr, cacheway[i].plen);
        cur +=len;
    }   

    return simple_read_from_buffer(buf, size, ppos, kbuf, (size_t)strlen(kbuf));
}

/* 
 * nr:addr@size
 */
static ssize_t cl_write (struct file *file, const char __user *buf, size_t size, loff_t *opps)
{
    char kbuf[16];
    char *cur,*tmp;
    phys_addr_t way,paddr, plen;
    
    if (copy_from_user(kbuf, buf, size)){
        return -EFAULT;
    }   
    
    cur = kbuf;
    way = memparse(cur, &tmp);
    if (tmp == cur){
        printk("cacheline lock : cache way expected!\n");
        return -EINVAL;
    }   
 
	 cur = tmp;
    if (*cur != ':'){
        printk("cacheline lock : ':' expected!\n");
        return -EINVAL;
    }

    cur++;
    paddr = memparse(cur, &tmp);
    if (tmp == cur){
        printk("cacheline lock : cache addr expected!\n");
        return -EINVAL;
    }

    cur = tmp;
    if (*cur != '@'){
        printk("cacheline lock : '@' expected!\n");
        return -EINVAL;
    }

    cur++;
    plen = memparse(cur, &tmp);
    if (tmp == cur){
        printk("cacheline lock : cache len expected!\n");
        return -EINVAL;
    }
    cacheway[way].paddr = paddr;
    cacheway[way].plen  = plen;
    printk("cache line lock : %llx:0x%llx@0x%llx\n", way, paddr, plen);

    return size;
}

static const struct file_operations cl_proc_fops = {
    .owner      = THIS_MODULE,
    .write      = cl_write,
    .read       = cl_read,
    .llseek     = default_llseek,
};

static int __init cl_init(void)
{
    struct proc_dir_entry *pde;

    pde = proc_create("cachelock", 0, NULL, &cl_proc_fops); 

    return 0;
}

static void __exit cl_exit(void)
{
    remove_proc_entry("cachelock", NULL);
}

module_init(cl_init);
module_exit(cl_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("chenyi<chenyi852@gmail.com");

