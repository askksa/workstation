ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-
PWD = $(shell pwd)
KDIR ?=/home/wenshuai.xi/SI/linux-3.18

export ARCH CROSS_COMPILE

obj-m = breakpoint.o

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	@rm -rf *.ko *.o *.mod.o *.order *.symvers *.mod.c .*.cmd .tmp_versions .*.tmp .*.d
