/* arm-none-linux-gnueabi-gcc -mfloat-abi=soft application_test.c -lrt -o soft.bin
 * arm-none-linux-gnueabi-gcc -mfloat-abi=softfp application_test.c -lrt -o softfp.bin
 * arm-none-linux-gnueabi-gcc -mfloat-abi=hard application_test.c -lrt  fail!!
 */
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

float Test1()
{
    float start_x; float start_y; float end_x; float end_y; float step; int step_count; float step_x; float step_y;
    float db_dis, dis_x, dis_y;

    start_x = 100;
    start_y = 100;
    end_x  = 0;
    end_y = 0;
    step = 0.003;

    dis_x             = end_x - start_x;
    dis_y             = end_y - start_y;
    db_dis            = sqrt(dis_x * dis_x + dis_y * dis_y);   //sqrt():开放运算（需要替换成你们的对应函数）
    step_count        = (int)(db_dis/step);              //divsp():单精度浮点除法运算（需要替换成你们的对应函数）
    step_x            = dis_x/step_count;
    step_y            = dis_y/step_count;

    return step_x+step_y;
}


//本函数运行1000次  看时间
float Test2()
{
    float x1,  y1,  x2,  y2;
    float xx1, yy1, xx2, yy2, L1, L2, val;

    x1 = 100.123 - 10.234;
    y1 = 100.4534 - 10.234;

    x2 = 10.234- 100.123;
    y2 = 10.234 - 10.4534;

    xx1 = x1*x1;
    yy1 = y1*y1;

    xx2 = x2*x2;
    yy2 = y2*y2;

    L1 = x1*x2 + y1*y2;
    L2 = (xx1 + yy1) * (xx2 + yy2);

    if(L1 >= 0.000001)
    {
        val = (L1*L1)/L2;      //divsp():单精度浮点除法运算（需要替换成你们的对应函数）
    }
    else
    {
        val = (L1 * L1)/L2;
    }

    if(val <= 0.5)
    {
        return -1;
    }

    return val;
}

double ln(double a)
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

int main(int argc,char *argv[])
{
    int t1;
    double Rt, Vadc;
    struct timespec time_start={0, 0},time_end={0, 0};
    int n=0;
    float ret1 = 0, ret2 = 0;
    int loop_cnt = atoi(argv[1]);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start);
#if 0
    /* 根据采样电压反推热敏电阻的温度 */
    for(Vadc=1; Vadc<1800; Vadc++) {
        Rt = Vadc * 47000 / (1800-Vadc);
        t1=1/(ln(Rt/10000)/3950+1/298.15)-273.15;
    }
#endif
    {
        for(n=0;n<1000*loop_cnt;n++)
        {
            ret1 = Test1();
            ret1+=ret1;
        }
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_end);

    printf("start time %ld s,%ld ns\n", time_start.tv_sec, time_start.tv_nsec);
    printf("end   time %ld s,%ld ns\n", time_end.tv_sec, time_end.tv_nsec);
    printf("loop_cnt:%d ret1:%d duration:%ld s %ld ns\n", n, (int)ret1, time_end.tv_sec-time_start.tv_sec, time_end.tv_nsec-time_start.tv_nsec);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start);
#if 0
    /* 根据采样电压反推热敏电阻的温度 */
    for(Vadc=1; Vadc<1800; Vadc++) {
        Rt = Vadc * 47000 / (1800-Vadc);
        t1=1/(ln(Rt/10000)/3950+1/298.15)-273.15;
    }
#endif
    {
        for(n=0;n<1000*loop_cnt;n++)
        {
            if((ret2=Test2()) == -1)
            {
                printf("exec err\n");
                break;
            }
            else
            {
                ret2+=ret2;
            }
        }
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_end);

    printf("start time %ld s,%ld ns\n", time_start.tv_sec, time_start.tv_nsec);
    printf("end   time %ld s,%ld ns\n", time_end.tv_sec, time_end.tv_nsec);
    printf("loop_cnt:%d ret2:%d duration:%ld s %ld ns\n", n, (int)ret2, time_end.tv_sec-time_start.tv_sec, time_end.tv_nsec-time_start.tv_nsec);
    printf("%.10fmv = %d\n", Vadc, t1);

    return 0;
}
