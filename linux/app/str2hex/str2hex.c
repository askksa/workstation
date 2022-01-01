#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void StringToHex(char *str, unsigned char *out)
{
    char *p = str;
    char high = 0, low = 0;
    int tmplen = strlen(p), cnt = 0;
    tmplen = strlen(p);
    while(cnt < (tmplen / 2))
    {
        high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
        low = (*(++ p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p) - 48 - 7 : *(p) - 48;
        out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
        p ++;
        cnt ++;
    }
    if(tmplen % 2 != 0) out[cnt] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
}

int main(int argc,char *argv[])
{
    int i = 0;
    long len = 0, ret = 0;
    char *filepath = argv[1];
    FILE *fp0,*fp1;
    char *str = NULL;
    char *str1,*token,*saveptr1;
    unsigned char out = 0;

    //open input/output file
    fp0 = fopen(filepath, "r+");
    if(!fp0){
        printf("open input file:%s failed\n",filepath);
        fclose(fp0);
        return -1;
    }
    fp1 = fopen("./out.bin", "w+");
    if(!fp1){
        printf("open output file failed\n");
        fclose(fp1);
        return -1;
    }

    //caculate input file size
    fseek(fp0, 0, SEEK_END);
    len = ftell(fp0);
    fseek(fp0, 0, SEEK_SET);
    printf("input file size:%ld\n",len);

    //malloc tmpbuff for read input file
    str = (char*)malloc(len);
    if(!str){
        printf("malloc failed\n");
        fclose(fp0);
        fclose(fp1);
        return -1;
    }
    ret = fread(str, len, 1, fp0);
    if(!ret){
        printf("read input file failed\n");
    }

    //parse str && save result to output file
    str1 = str;
    while(*str1 != '\0'){
        if((*str1 == '0') && ((*(str1+1) == 'x') || (*(str1+1) == 'X'))){
            *str1 = 'x';
            str1 = str1+2;
        }
        else
            str1++;
    }

    for(str1 = str; ;str1 = NULL){
        token = strtok_r(str1, ",xX", &saveptr1);
        if(token){
            StringToHex(token,&out);
            ret = fwrite(&out, 1, 1, fp1);
            if(!ret){
                printf("write out file failed\n");
                goto exit;
            }
        }
        else
            break;
    }
    printf("transform complete\n");

exit:
    free(str);
    fclose(fp0);
    fclose(fp1);

    return 0;
}
