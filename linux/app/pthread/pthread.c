#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAJOR_VERSION 2
#define SUB_VERSION 3

#define MACRO_TO_STR(macro) #macro
#define VERSION_STR(major_version,sub_version) ({char *tmp = sub_version/100 ? \
                                    "api_version_"MACRO_TO_STR(major_version)"."MACRO_TO_STR(sub_version) : sub_version/10 ? \
                                    "api_version_"MACRO_TO_STR(major_version)".0"MACRO_TO_STR(sub_version) : \
                                    "api_version_"MACRO_TO_STR(major_version)".00"MACRO_TO_STR(sub_version);tmp;})

#define API_VERSION VERSION_STR(MAJOR_VERSION,SUB_VERSION)

#define check_condition(condition) \
({\
    int _condition = condition;\
    if(_condition > 1)\
        _condition = 1;\
    _condition;\
 })
#define _test_macro(string,condition)  \
({\
    if(condition)\
        printf("condition is true string:%s\n",string);\
 })
#define test_macro(string,condition) _test_macro(string,check_condition(condition))

#define ChnId2LayerPortId(chnid, layerid, portid) {\
    \
    if(chnid <= 15)\
    {\
        layerid = 0;\
        portid = chnid;\
    }\
    else if(chnid == 16)\
    {\
        layerid = 1;\
        portid = 0;\
    }\
    else\
        printf("chnid is ivalid\n");\
}

#define pthread_test 0
#if (pthread_test == 1)
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct node {
    int n_number;
    struct node *n_next;
} *head = NULL;

static void cleanup_handler(void *arg)
{
    printf("Cleanup handler of second thread\n");
    free(arg);
    pthread_mutex_unlock(&mtx);
}
static void *thread_func(void *arg)
{
    struct node *p = NULL;

    pthread_cleanup_push(cleanup_handler, p);
    while (1) {
        pthread_mutex_lock(&mtx);
        while (head == NULL){
            pthread_cond_wait(&cond, &mtx);
        }
        p = head;
        head = head->n_next;
        printf("Got %d from front of queue\n", p->n_number);
        free(p);
        pthread_mutex_unlock(&mtx);
    }
    pthread_cleanup_pop(0);
    return 0;
}

int main(void)
{
    pthread_t tid;
    int i;
    struct node *p;

    pthread_create(&tid, NULL, thread_func, NULL);
    for (i = 0; i < 10; i++) {
        p = malloc(sizeof(struct node));
        p->n_number = i;
        pthread_mutex_lock(&mtx);
        p->n_next = head;
        head = p;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);
        sleep(1);
    }
    printf("thread 1 wanna end the line.So cancel thread 2.\n");
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    printf("All done -- exiting\n");
    return 0;
}
#else

void *mid_thread(void *arg);
void *term_thread(void *arg);

int main()
{
    pthread_t mid_tid, term_tid;

    if(pthread_create(&mid_tid, NULL, mid_thread, NULL))
    {
        perror("create mid thread error!\n");
        return 0;
    }
    if(pthread_create(&term_tid, NULL, term_thread, &mid_tid))
    {
        perror("create term thread error!\n");
        return 0;
    }
    if(pthread_join(mid_tid, NULL))
    {
        perror("wait mid thread  error!\n");
        return 0;
    }
    if(pthread_join(term_tid, NULL))
    {
        perror("wait term thread error!\n");
        return 0;
    }

    return 0;
}
void *mid_thread(void *arg)
{
    int times = 0;
    printf("mid thread created!\n");

    while(1)
    {
        printf("wait term thread %d times!\n",times);
        usleep(200*1000);
        times++;
    }
}
void *term_thread(void *arg)
{
    pthread_t *tid;

    printf("term thread created!\n");
    sleep(5);
    if(NULL != arg)
    {
        tid = arg;
        pthread_cancel(*tid); //cancel mid thread
    }
}
#endif
