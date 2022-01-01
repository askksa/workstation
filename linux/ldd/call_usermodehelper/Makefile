KDIR?=$(HOME)/SI/master_alkaid/kernel
ARCH?=arm
CROSS_COMPILE?=arm-linux-gnueabihf-
PWD=$(shell pwd)

obj-m=call_usermodehelper.o
all:
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
clean:
	@rm -rf *.ko *.o *.mod.o *.order *.symvers *.mod.c .*.cmd .tmp_versions .*.tmp .*.d

