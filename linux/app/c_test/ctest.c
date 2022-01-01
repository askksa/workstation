#include <stdio.h>
#include <ctype.h>

char str[] = {'a',' ','\t','\r','\n','\v','\f','\0','b'};

int main(void)
{
    FILE *fp;
    unsigned int i = 0;
    char buff[20]={0};

    fp = fopen("./test.txt","rt");

    if(fp)
    {
        fread(buff,20,1,fp);
    }
    else
    {
        printf("open file failed\n");
    }

    for(i=0;i<20;i++)
    {
        printf("buff[%d]:%x\n",i,buff[i]);
    }

    for(i=0;i<sizeof(str);i++)
    {
        if(isspace(str[i]))
        {
            printf("null character\n");
        }
        else
        {
            printf("str[%d]:%x\n",i,str[i]);
        }
    }
    return 0;
}
