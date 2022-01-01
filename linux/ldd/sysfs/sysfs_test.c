#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <linux/fs.h>
#include <linux/of_irq.h>

#define SYSFS_TEST_DEVICE_COUNT    1
#define SYSFS_TEST_DEVICE_NAME     "sysfstest"
#define SYSFS_TEST_CLASS_NAME "sysfs_test_class"

typedef struct
{
    int s32Major;
    int s32Minor;
    struct cdev cdev;
    const struct file_operations fops;
    struct device *devicenode;
}stSysfsTestCtx_t;

static stSysfsTestCtx_t stSysfsTestCtx =
{
    .s32Major = 0,
    .s32Minor = 0,
    .devicenode = NULL,
    .cdev = {
        .owner = THIS_MODULE,
        .kobj = {.name = SYSFS_TEST_DEVICE_NAME, },
    },
};

static struct class *_gpstSysfsTestClass = NULL;

ssize_t sysfstest_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    printk("%s:%d\n",__FUNCTION__,__LINE__);
    return 0;
}
ssize_t sysfstest_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    printk("%s:%d\n",__FUNCTION__,__LINE__);
    return 0;
}
DEVICE_ATTR(sysfstest,0664,sysfstest_show,sysfstest_store);

int sysfs_test_init(void)
{
    int ret = 0;
    dev_t dev;

    printk("%s:%d\n",__FUNCTION__,__LINE__);

    if(stSysfsTestCtx.s32Major == 0)
    {
        ret = alloc_chrdev_region(&dev, stSysfsTestCtx.s32Minor, SYSFS_TEST_DEVICE_COUNT, SYSFS_TEST_DEVICE_NAME);
        stSysfsTestCtx.s32Major  = MAJOR(dev);
    }

    _gpstSysfsTestClass = class_create(THIS_MODULE, SYSFS_TEST_CLASS_NAME);
    if(IS_ERR(_gpstSysfsTestClass))
    {
        printk("Failed at class_create().Please exec [mknod] before operate the device/n");
    }
    cdev_init(&stSysfsTestCtx.cdev, &stSysfsTestCtx.fops);
    if (0 != (ret = cdev_add(&stSysfsTestCtx.cdev, dev, SYSFS_TEST_DEVICE_COUNT)))
    {
        printk( "Unable add a character device\n");
    }
    if(stSysfsTestCtx.devicenode == NULL)
    {
        stSysfsTestCtx.devicenode = device_create(_gpstSysfsTestClass, NULL, dev, NULL, SYSFS_TEST_DEVICE_NAME);
        device_create_file(stSysfsTestCtx.devicenode, &dev_attr_sysfstest);
    }
    printk("%s:%d\n",__FUNCTION__,__LINE__);

    return ret;
}

void sysfs_test_exit(void)
{
    if(stSysfsTestCtx.cdev.count)
        cdev_del(&stSysfsTestCtx.cdev);
    if(stSysfsTestCtx.devicenode != NULL)
        device_unregister(stSysfsTestCtx.devicenode);
    if(_gpstSysfsTestClass != NULL)
        class_destroy(_gpstSysfsTestClass);
    _gpstSysfsTestClass = NULL;

    printk("%s:%d\n",__FUNCTION__,__LINE__);
}

module_init(sysfs_test_init);
module_exit(sysfs_test_exit);

MODULE_AUTHOR("wenshuai.xi");
MODULE_DESCRIPTION("sysfs test sample");
MODULE_LICENSE("GPL");
