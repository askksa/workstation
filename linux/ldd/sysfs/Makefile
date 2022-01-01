KDIR?=/home/wenshuai.xi/SI/linux-3.18
ARCH?=arm
CROSS_COMPILE?=arm-linux-gnueabihf-
PWD=$(shell pwd)

obj-m=sysfs_test.o
all:
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
clean:
	@rm -rf *.ko *.o *.mod.o *.order *.symvers *.mod.c .*.cmd .tmp_versions .*.tmp .*.d

