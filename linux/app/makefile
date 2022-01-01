objs:=$(patsubst %/,%,$(shell ls -d */))

export CROSS_COMPILE

.PHONY:$(objs) all clean

all:
	for dirname in $(objs);\
	do\
		make -f makefile.build obj=$$dirname;\
	done

$(objs):
	make -f makefile.build obj=$@

clean:
	for dirname in $(objs);\
	do\
		make -f makefile.clean obj=$$dirname;\
	done
