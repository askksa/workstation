CROSS_COMPILE ?= arm-linux-gnueabihf-
CC := $(CROSS_COMPILE)gcc
SRCS = $(filter %.c, $(shell ls))
OBJS = $(patsubst %.c, %.o, $(SRCS))
OUT_BIN = watchdog-simple

all:${OUT_BIN}

${OUT_BIN}:${OBJS}
	${CC} $< -o $@
${OBJS}:${SRCS}
	${CC} -c $< -o $@
clean:
	@rm -fr ${OBJS} $(OUT_BIN)
