KDIR?=/home/wenshuai.xi/github/linux-4.9.10
ARCH?=arm
CROSS_COMPILE?=arm-linux-gnueabihf-
PWD=$(shell pwd)
GCC?=${CROSS_COMPILE}gcc

obj-m=performance.o
all:
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules

clean:
	@rm -rf *.ko *.o *.mod.o *.order *.symvers *.mod.c .*.cmd .tmp_versions .*.tmp .*.d ${APP}

