GCC?=arm-linux-gnueabihf-gcc
SRC:=$(filter %.c, $(shell ls))
BIN:=$(patsubst %.c, %, $(SRC))

.PHONY:clean all

all:$(BIN)

$(BIN):%:%.c
	$(GCC) -o $@ $^

clean:
	@rm -rf $(BIN)
