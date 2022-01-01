#include <linux/types.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/slab.h>

struct el {
    struct list_head lp;
    int key;
    spinlock_t mutex;
    int data;
};
spinlock_t listmutex;
LIST_HEAD(head);

struct task_struct *pRearderThread;
struct task_struct *pWriterThread;
struct task_struct *pUpdaterThread;

int search(int key, int *result)
{
    struct el *p;

    rcu_read_lock();
    list_for_each_entry_rcu(p, &head, lp) {
        if (p->key == key) {
            *result = p->data;
            rcu_read_unlock();
            return 1;
        }
    }
    rcu_read_unlock();
    return 0;
}

int delete(int key)
{
    struct el *p;

    spin_lock(&listmutex);
    list_for_each_entry_rcu(p, &head, lp) {
        if (p->key == key) {
            list_del_rcu(&p->lp);
            spin_unlock(&listmutex);
            printk("list del node:%d\n",key);
            synchronize_rcu();
            kfree(p);
            return 1;
        }
    }
    spin_unlock(&listmutex);
    return 0;
}
int ReaderThread(void *data)
{
    int n = 0;
    int result;

    msleep(10);
    while(n<10)
    {
        if(search(n,&result))
            printk("searched:%d\n",n);
        else
            printk("no found\n");
        n++;
    }

    return 0;
}
int UpdateThread(void *data)
{
    int n =0;

    msleep(100);
    while(n<10){
        if(delete(n))
            printk("delete:%d\n",n);
        n++;
        msleep(500);
    }

    return 0;
}

int WriterThread(void *data)
{
    int n=0;
    struct el *pNewData;

    while(n<10){
        pNewData = kmalloc(sizeof(struct el),GFP_KERNEL);
        if(pNewData)
        {
            pNewData->key = n;
            list_add_rcu(&pNewData->lp,&head);
            n++;
        }
        else
            printk("kmalloc failed, try again\n");
    }
    while(1)
    {
        static long long int cnt = 0;
        rcu_read_lock();
        while(cnt++ < 1000000000);
        rcu_read_unlock();
        break;
    }
    return 0;
}

static int rcu_test_init(void)
{
    printk("module init\n");

    pRearderThread = kthread_run(ReaderThread,NULL,"ReaderThread");
    pWriterThread = kthread_run(WriterThread,NULL,"WriterThread");
    pUpdaterThread = kthread_run(UpdateThread,NULL,"UpdateThread");

    return 0;
}

static void rcu_test_exit(void)
{
    printk("module exit\n");
}

module_init(rcu_test_init);
module_exit(rcu_test_exit);
MODULE_LICENSE("GPL");

