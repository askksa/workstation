#include "linux/module.h"
#include "linux/kthread.h"
#include "linux/device.h"
#include "linux/fs.h"

#define PERF_TIME(pu64Time) do {\
        struct timespec sttime;\
        memset(&sttime, 0, sizeof(sttime));\
        ktime_get_ts(&sttime);\
         *(pu64Time) = ((long long int)sttime.tv_sec) * 1000000ULL + (sttime.tv_nsec / 1000);\
    } while(0)

static struct task_struct *wait_thread = NULL;
static struct timer_list wakeup_timer;
static atomic_t wakeupflag = ATOMIC_INIT(0);
DECLARE_WAIT_QUEUE_HEAD(waitqueue_head);

static int wait_thread_func(void *data)
{
    int waitcnt = 0;
    long long int au64BeforeTimeOut[10];
    long long int au64AfterTimeOut[10];
    while(!kthread_should_stop())
    {
        PERF_TIME(&au64BeforeTimeOut[waitcnt]);
        //wait_event_interruptible_timeout(waitqueue_head, false, msecs_to_jiffies(1000));
        wait_event_interruptible_timeout(waitqueue_head, atomic_read(&wakeupflag)>0, msecs_to_jiffies(1000));
        PERF_TIME(&au64AfterTimeOut[waitcnt]);
        atomic_dec(&wakeupflag);
        printk("[func:%s][line:%d] waitcnt:%d\n",__func__,__LINE__,waitcnt);
        if(waitcnt++ == 10)
            break;
    }
    for(waitcnt = 0; waitcnt < 10; waitcnt++)
    {
        printk("timeout:%d take time:%lld\n",waitcnt, au64AfterTimeOut[waitcnt]-au64BeforeTimeOut[waitcnt]);
    }
    return 0;
}

static void wakeup_timer_handle(unsigned long arg)
{
    atomic_inc(&wakeupflag);
    wake_up_interruptible(&waitqueue_head);
    mod_timer(&wakeup_timer, jiffies+msecs_to_jiffies(10));
}

static int wait_queue_test_init(void)
{
    setup_timer(&wakeup_timer, wakeup_timer_handle, 0);
    mod_timer(&wakeup_timer, jiffies+msecs_to_jiffies(10));
    wait_thread = kthread_run(wait_thread_func, NULL, "wait_thread");

    printk("module init\n");
    return 0;
}

static void wait_queue_test_exit(void)
{
    kthread_stop(wait_thread);
    del_timer(&wakeup_timer);

    printk("module exit\n");
}

module_init(wait_queue_test_init);
module_exit(wait_queue_test_exit);
MODULE_LICENSE("GPL");

