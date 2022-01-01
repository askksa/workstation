#include "test.h"

test_t g_test;
bd_t g_bd;

global_ptr

int main(int argc, char* argv[])
{
    test = &g_test;
    test->bd = &g_bd;

    printf("register var test\n");

    return 0;
}
