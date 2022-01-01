#include <stdio.h>
#include <stdlib.h>
#include "listops/listops.h"

LIST_HEAD(list_test);

typedef struct test_s
{
    int num;
    struct list_head node;
}test_t;

int main(void)
{
    int i;
    struct list_head *pos,*n;

    for(i=0;i<10;i++)
    {
        test_t *ptest;
        ptest = malloc(sizeof(test_t));
        if(ptest)
        {
            ptest->num = i;
            list_add_tail(&ptest->node,&list_test);
        }
    }

    list_for_each_safe(pos,n,&list_test){
        test_t *ptest;
        ptest = list_entry(pos, test_t, node);
        printf("list entry data:%d\n",ptest->num);
        list_del(pos);
        free(ptest);
    }

    printf("test end\n");

    return 0;
}
