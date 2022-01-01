#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/sched.h>

static __init int hello_init(void)
{
    int result = 0;
    char cmd_path[] = "/mnt/run/i2m/test";
    char *cmd_argv[] = {cmd_path,NULL};
    char *cmd_envp[] = {NULL};

    printk("module init\n");
    result = call_usermodehelper(cmd_path, cmd_argv, cmd_envp, UMH_WAIT_PROC);
    printk("THe result of call_usermodehelper is %d\n", result);

    return result;
}

static __exit void hello_exit(void)
{
    printk("module exit\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
