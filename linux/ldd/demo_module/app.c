#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char ioctl_buff[256] = "hello world";
enum ioctl_cmd{
    E_CMD_1,
    E_CMD_2,
    E_CMD_3,
    E_CMD_MAX,
};
#define ioctl_cmd_1 _IO('k',E_CMD_1)
#define ioctl_cmd_2 _IOR('k',E_CMD_2,ioctl_buff)
#define ioctl_cmd_3 _IOW('k',E_CMD_3,ioctl_buff)

int main(int argc, char **argv)
{
    int fd;
    int i;
    char data[256] = {0};
    int retval;
    char *name = "/dev/mymodule";

    if(argc == 2)
    {
        name = argv[1];
    }

    fd = open(name, O_RDWR | O_CREAT);
    if (fd == -1) {
        perror("open error\n");
        exit(-1);
    }
    printf("open %s successfully\n", name);

    retval = write(fd, "abccba", 7);
    if (retval == -1) {
        perror("write error\n");
        close(fd);
        exit(-1);
    }
    retval = lseek(fd, 0, 0);
    if (retval == -1) {
        perror("lseek error\n");
        close(fd);
        exit(-1);
    }
    retval = read(fd, data, 10);
    if (retval == -1) {
        perror("read error\n");
        close(fd);
        exit(-1);
    }
    printf("read successfully: %s\n", data);

    retval = ioctl(fd, ioctl_cmd_1, NULL);
    if (retval) {
        perror("ioctl error\n");
        close(fd);
        exit(-1);
    }

    retval = ioctl(fd, ioctl_cmd_3, ioctl_buff);
    if (retval) {
        perror("ioctl error\n");
        close(fd);
        exit(-1);
    }

    retval = ioctl(fd, ioctl_cmd_2, ioctl_buff);
    if (retval) {
        perror("ioctl error\n");
        close(fd);
        exit(-1);
    }
    else
        printf("ioctl_buff:%s\n",ioctl_buff);

    close(fd);
    return 0;
}

