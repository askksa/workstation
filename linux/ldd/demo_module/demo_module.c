#include "linux/module.h"
#include "linux/kthread.h"
#include "linux/device.h"
#include "linux/cdev.h"
#include "linux/fs.h"
#include "asm/uaccess.h"

#define DEV_NAME "mymodule"

static char *name = "world";
static int count = 2;
static int para[8] = {1,2,3,4};
static int n_para = 8;
module_param(count, int, S_IRUGO);
module_param(name, charp, S_IRUGO);
module_param_array(para , int , &n_para , S_IRUGO);
//insmod moduleparam_test.ko count=6 name="module" para=10,2,8,2,3,9

static int wakeupcnt=0;
static unsigned char demo_inc = 0;
static char demo_buffer[256];
static struct task_struct *mythread = NULL;
static struct timer_list mytimer;
static atomic_t wakeupflag = ATOMIC_INIT(0);
DECLARE_WAIT_QUEUE_HEAD(mywaitqueue);

struct demo_cdev
{
    dev_t dev;
    int major_num;
    int minor_num;
    struct cdev cdev;
    struct device *mydevice;
    struct class *myclass;
}demo_cdev_res;

enum ioctl_cmd{
    E_CMD_1,
    E_CMD_2,
    E_CMD_3,
    E_CMD_MAX,
};

typedef int (*WrapperFunction)(void *arg);
static int ioctl_func_1(void *arg)
{
    printk("this is ioctl_func_1\n");
    return 0;
}

static int ioctl_func_2(void *arg)
{
    int result = 0;
    if(arg)
    {
        struct
        {
            int len;
            void *ptr;
        }_arg;
        memcpy(&_arg, arg, sizeof(_arg));
        strcpy(demo_buffer, "this is ioctl_func_2");
        result = copy_to_user((char*)_arg.ptr, demo_buffer, _arg.len);
        if(result)
        {
            printk("copy_to_user fail\n");
            return -EFAULT;
        }
    }
    return 0;
}

static int ioctl_func_3(void *arg)
{
    int result = 0;
    if(arg)
    {
        struct
        {
            int len;
            void *ptr;
        }_arg;
        memcpy(&_arg, arg, sizeof(_arg));
        result = copy_from_user(demo_buffer, (char*)_arg.ptr, _arg.len);
        if(result)
        {
            printk("copy_from_user fail\n");
            return -EFAULT;
        }
        else
            printk("demo_buffer:%s\n",demo_buffer);
    }
    return 0;
}
WrapperFunction ioctl_table[E_CMD_MAX] = {
    [E_CMD_1] = ioctl_func_1,
    [E_CMD_2] = ioctl_func_2,
    [E_CMD_3] = ioctl_func_3,
};

static int demo_module_open(struct inode *inode, struct file *file)
{
    int i=0;
    struct demo_cdev *dev;

    if (demo_inc > 0)
        return -ERESTARTSYS;
    demo_inc++;

    dev = container_of(inode->i_cdev, struct demo_cdev, cdev);
    file->private_data = dev;

    for (i = 0; i < count; i++)
        printk(KERN_ALERT "(%d) Hello, %s ！\n", i, name);

    for(i = 0; i < n_para; i++)
        printk(KERN_ALERT "para[%d] : %d \n", i, para[i]);

    printk("[func:%s][line:%d]\n",__func__,__LINE__);
    return 0;
}

static ssize_t demo_module_read(struct file *file, char __user *user, size_t size, loff_t *offset)
{
    int result;
    loff_t pos = *offset;

    if (pos >= 256){
        result = 0;
        goto out;
    }
    if (size > (256 - pos))
        size = 256 - pos;
    pos += size;

    if (copy_to_user(user, demo_buffer + *offset, size)){
        size = -EFAULT;
        goto out;
    }
    *offset = pos;

    printk("[func:%s][line:%d]\n",__func__,__LINE__);

out:
    return size;
}
static ssize_t demo_module_write(struct file *file, const char __user *user, size_t size, loff_t *offset)
{
    ssize_t retval = -ENOMEM;
    loff_t pos = *offset;

    if (pos > 256)
        goto out;
    if (size > (256 - pos))
        size = 256 - pos;

    pos += size;
    if (copy_from_user(demo_buffer + *offset, user, size)) {
        retval = -EFAULT;
        goto out;
    }

    *offset = pos;
    retval = size;

    printk("[func:%s][line:%d]\n",__func__,__LINE__);
out:
    return retval;
}

static long demo_module_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int err = 0;
    int cmdidx = _IOC_NR(cmd);

    if(_IOC_TYPE(cmd) == 'k' && cmdidx >= 0 && cmdidx < E_CMD_MAX)
    {
        if(arg)
        {
            if(_IOC_DIR(cmd)&_IOC_WRITE)
            {
                err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
            }
            else if(_IOC_DIR(cmd)&_IOC_READ)
            {
                err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
            }
            else
            {
                printk("ioctl cmd is invalid\n");
                return -EINVAL;
            }
            if(err)
                return -EFAULT;
            else
            {
                struct
                {
                    int len;
                    void *ptr;
                }_arg = {_IOC_SIZE(cmd), (void*)arg};
                err = ioctl_table[cmdidx](&_arg);
            }
            if(err)
                return -EFAULT;
        }
        else
            ioctl_table[cmdidx](NULL);
        return 0;
    }

    printk("demo_ioctl error\n");
    return -EFAULT;
}

static loff_t demo_module_llseek(struct file *filp, loff_t off, int whence)
{
    loff_t pos;

    pos = filp->f_pos;
    switch (whence) {
        case 0:
            pos = off;
            break;
        case 1:
            pos += off;
            break;
        case 2:
        default:
            return -EINVAL;
    }

    if ((pos > 256) || (pos < 0))
        return -EINVAL;

    printk("demo_llseek successfully\n");

    return filp->f_pos = pos;
}

static int demo_module_close(struct inode *node, struct file *file)
{
    demo_inc--;
    printk("[func:%s][line:%d]\n",__func__,__LINE__);
    return 0;
}

static int thread_func(void *data)
{
    while(!kthread_should_stop())
    {
        wait_event_interruptible_timeout(mywaitqueue, atomic_read(&wakeupflag)>0, msecs_to_jiffies(100));
        if(atomic_read(&wakeupflag)>0)
            atomic_dec(&wakeupflag);

        if(++wakeupcnt <= 10)
            printk("[func:%s][line:%d][wakeupcnt:%d]\n",__func__,__LINE__,wakeupcnt);
    }
    return 0;
}
static void timer_handle(unsigned long arg)
{
    atomic_inc(&wakeupflag);
    wake_up_interruptible(&mywaitqueue);
    mod_timer(&mytimer, jiffies+HZ);
}

static struct file_operations fops = {

    .owner = THIS_MODULE,
    .open = demo_module_open,
    .read = demo_module_read,
    .write = demo_module_write,
    .llseek = demo_module_llseek,
    .unlocked_ioctl = demo_module_ioctl,
    .release = demo_module_close,
};


static int demo_module_init(void)
{
    int result;
    double dvar = 1.2;
    float fvar = 1.3;
    double retval = dvar+fvar;

    printk("dvar+fvar=%f\n",retval);

    result = alloc_chrdev_region(&demo_cdev_res.dev, 0, 1, DEV_NAME);
    if(result)
    {
        printk("alloc_chrdev_region fail\n");
        return result;
    }
    cdev_init(&demo_cdev_res.cdev, &fops);
    result = cdev_add(&demo_cdev_res.cdev, demo_cdev_res.dev, 1);
    if(result)
    {
        printk("cdev add fail\n");
        return result;
    }
    demo_cdev_res.major_num = MAJOR(demo_cdev_res.dev);
    demo_cdev_res.minor_num = MINOR(demo_cdev_res.dev);

    demo_cdev_res.myclass = class_create(THIS_MODULE, "mymodules");
    demo_cdev_res.mydevice = device_create(demo_cdev_res.myclass, NULL, demo_cdev_res.dev, NULL, "mymodule");

    setup_timer(&mytimer, timer_handle, 0);
    mod_timer(&mytimer, jiffies+HZ);
    mythread = kthread_run(thread_func, NULL, "mythread");

    printk("module init\n");
    return 0;
}

static void demo_module_exit(void)
{
    cdev_del(&demo_cdev_res.cdev);
    unregister_chrdev_region(demo_cdev_res.dev, 1);
    device_destroy(demo_cdev_res.myclass, demo_cdev_res.dev);
    class_destroy(demo_cdev_res.myclass);

    kthread_stop(mythread);
    del_timer(&mytimer);


    printk("module exit\n");
}

module_init(demo_module_init);
module_exit(demo_module_exit);
MODULE_LICENSE("GPL");

