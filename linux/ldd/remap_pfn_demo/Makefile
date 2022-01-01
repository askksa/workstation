KDIR ?= /home/wenshuai.xi/github/linux/
ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-
PWD = $(shell pwd)

obj-m:=remap_pfn_alloc_pages.o
obj-m+=remap_pfn_kmalloc.o
obj-m+=remap_pfn_vmalloc.o

all:
	make -C ${KDIR} ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} M=${PWD} modules
clean:
	@rm -rf *.ko *.o *.mod.o *.order *.symvers *.mod.c .*.cmd .tmp_versions .*.tmp .*.d
