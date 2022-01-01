KDIR ?= /home/wenshuai.xi/SI/linux-4.9.137/
ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-
PWD = $(shell pwd)

obj-m:=kdb_hello.o

all:
	make -C ${KDIR} ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} M=${PWD} modules
clean:
	@rm -rf *.ko *.o *.mod.o *.order *.symvers *.mod.c .*.cmd .tmp_versions .*.tmp .*.d
