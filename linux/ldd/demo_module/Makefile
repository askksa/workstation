KDIR?=/home/wenshuai.xi/github/linux-4.9.10
ARCH?=arm
CROSS_COMPILE?=arm-linux-gnueabihf-
PWD=$(shell pwd)
GCC?=${CROSS_COMPILE}gcc
APP=app
app_src=app.c

obj-m=demo_module.o
all:${APP}
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules

${APP}:
	${GCC} -o $@ ${app_src}
clean:
	@rm -rf *.ko *.o *.mod.o *.order *.symvers *.mod.c .*.cmd .tmp_versions .*.tmp .*.d ${APP}

