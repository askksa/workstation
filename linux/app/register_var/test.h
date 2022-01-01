#include <stdio.h>

typedef struct bd_s
{
    int data;
}bd_t;

typedef struct test_s
{
    bd_t *bd;
    char num;
}test_t;

void func(void);

#define global_ptr register volatile test_t *test asm ("r8");

