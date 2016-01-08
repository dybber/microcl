OS := $(shell uname -s)
is_64=$(shell s=`uname -m`; if (echo $$s | grep x86_64 > /dev/null); then echo 1; fi)

ifeq ($(OS),Darwin)
    CC=clang
    LIB=
    CFLAGS=-Wall -W -O2 -framework OpenCL
    INCLUDES=
else
    CC=gcc
    CFLAGS=-Wall -W -O2 -std=c99
    LIB=-lOpenCL
endif

.PHONY: all
all: libmcl.a

libmcl.o: mcl.c
	cc $(CFLAGS) $(INCLUDES) -o $@ -c $^ $(LIB)

libmcl.a: libmcl.o
	ar -rucv $@ $^

.PHONE: clean
clean:
	rm libmcl.o
	rm libmcl.a
