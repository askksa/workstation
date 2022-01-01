#include "linux/module.h"
#include "linux/kthread.h"
#include "linux/device.h"
#include "linux/cdev.h"
#include "linux/fs.h"
#include "linux/time.h"
#include "linux/delay.h"
#include "asm/uaccess.h"

#define DEV_NAME "performance"

static struct task_struct *mythread = NULL;
static struct timer_list mytimer;
static atomic_t wakeupflag = ATOMIC_INIT(0);
DECLARE_WAIT_QUEUE_HEAD(mywaitqueue);

#define PERFORMANCE_DBG(format,...) ({do{printk(KERN_DEBUG "[%s][%d]"format,__FUNCTION__,__LINE__,__VA_ARGS__);}while(0);})

#define GET_TIME_NSEC(savensec) {\
\
    struct timespec ts;\
    memset(&ts,0,sizeof(struct timespec));\
    ktime_get_ts(&ts);\
    *(long long*)(&savensec) = timespec_to_ns(&ts);\
}

static int thread_func(void *data)
{
    long long before,after;
    while(!kthread_should_stop())
    {
        GET_TIME_NSEC(before);
        wait_event_interruptible_timeout(mywaitqueue, atomic_read(&wakeupflag)>0, msecs_to_jiffies(30));
        GET_TIME_NSEC(after);
        PERFORMANCE_DBG("ns delta:%lld\n",after-before);

        if(atomic_read(&wakeupflag)>0)
            atomic_dec(&wakeupflag);
    }
    return 0;
}
static void timer_handle(unsigned long arg)
{
    atomic_inc(&wakeupflag);
    wake_up_interruptible(&mywaitqueue);
    mod_timer(&mytimer, jiffies+HZ);
}

static int performance_init(void)
{
    setup_timer(&mytimer, timer_handle, 0);
    mod_timer(&mytimer, jiffies+HZ);
    mythread = kthread_run(thread_func, NULL, "mythread");

    printk("module init\n");
    return 0;
}

static void performance_exit(void)
{
    kthread_stop(mythread);
    del_timer(&mytimer);

    printk("module exit\n");
}

module_init(performance_init);
module_exit(performance_exit);
MODULE_LICENSE("GPL");

