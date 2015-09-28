CC ?= gcc

CFLAGS ?= -Wall -Wextra -pedantic -std=gnu99

C_COMPILE = $(CC) $(CFLAGS) -c
C_LINK = $(CC) $(CFLAGS)

.PHONY: all
all: build build/thread build/timer

build:
	mkdir build

build/timer: timer/main.c
	$(C_LINK) timer/main.c -o build/timer

build/thread: thread/main.c
	$(C_LINK) -pthread thread/main.c -o build/thread

.PHONY: clean
clean:
	find build -not -type d -delete
