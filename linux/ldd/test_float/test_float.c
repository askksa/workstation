#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/neon.h>

static double ln(double a)
{
    int N = 15;
    int k,nk;
    double x,xx,y;

    x = (a-1)/(a+1);
    xx = x*x;
    nk = 2*N+1;
    y = 1.0/nk;
    for(k=N;k>0;k--) {
        nk = nk - 2;
        y = 1.0/nk+xx*y;
    }

    return 2.0*x*y;
}

static int hello_init(void)
{
    int t1, force2int;
    double Rt, Vadc;
    struct timespec time_start={0, 0},time_end={0, 0};
    double a;
    a = 3.9;
    a = a*a;
    printk("Hello,world! %f\n", a);
    printk("Hello,world! %d\n", (int)a);

    time_start = current_kernel_time();
    kernel_neon_begin();
    /* 根据采样电压反推热敏电阻的温度 */
    for(Vadc=1; Vadc<1800; Vadc++) {
        Rt = Vadc * 47000 / (1800-Vadc);
        t1=1/(ln(Rt/10000)/3950+1/298.15)-273.15;
    }
    kernel_neon_end();
    time_end = current_kernel_time();

    printk("start time %ld s,%ld ns\n", time_start.tv_sec, time_start.tv_nsec);
    printk("end   time %ld s,%ld ns\n", time_end.tv_sec, time_end.tv_nsec);
    printk("duration:%ld s %ld ns\n", time_end.tv_sec-time_start.tv_sec, time_end.tv_nsec-time_start.tv_nsec);

    force2int = (int)Vadc;
    printk("%d = %d\n", force2int, t1);

    return 0;
}

static void hello_exit(void)
{
    printk("Goodbye,cruel world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Vedic <FZKmxcz@163.com>");
MODULE_LICENSE("Dual BSD/GPL");
