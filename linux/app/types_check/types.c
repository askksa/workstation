#include <stdio.h>

int main(void)
{
    printf("\ncheck char\n");
    printf("char:%ld\n",sizeof(char));
    printf("signed char:%ld\n",sizeof(signed char));
    printf("unsigned char:%ld\n",sizeof(unsigned char));

    printf("\ncheck int\n");
    printf("int:%ld\n",sizeof(int));
    printf("signed int:%ld\n",sizeof(signed int));
    printf("unsigned int:%ld\n",sizeof(unsigned int));

    printf("\ncheck long int\n");
    printf("long int:%ld\n",sizeof(long int));
    printf("signed long int:%ld\n",sizeof(signed long int));
    printf("unsigned long int:%ld\n",sizeof(unsigned long int));

    printf("\ncheck long long int\n");
    printf("long long int:%ld\n",sizeof(long long int));
    printf("signed long long int:%ld\n",sizeof(signed long long int));
    printf("unsigned long long int:%ld\n",sizeof(unsigned long long int));

    printf("\ncheck float\n");
    printf("float:%ld\n",sizeof(float));

    printf("\ncheck double\n");
    printf("double:%ld\n",sizeof(double));

    return 0;
}
