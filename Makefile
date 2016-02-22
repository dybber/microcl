OS := $(shell uname -s)
is_64=$(shell s=`uname -m`; if (echo $$s | grep x86_64 > /dev/null); then echo 1; fi)

ifeq ($(OS),Darwin)
    CC=clang
    CFLAGS=-Wall -W -O2 -framework OpenCL -D_DEBUG
    LIB=
    INCLUDES=-I/usr/local/cuda/include
else
    CC=gcc
    CFLAGS=-Wall -W -O2 -std=c99 -D_DEBUG
    LIB=-lOpenCL
    INCLUDES=-I/usr/local/cuda/include
endif

.PHONY: all
all: libmcl.a

libmcl.o: mcl.c mcl_device_info.h mcl.h
	cc $(CFLAGS) $(INCLUDES) -o $@ -c mcl.c $(LIB)

libmcl.a: libmcl.o
	ar -rucv $@ $^

.PHONE: clean
clean:
	rm libmcl.o
	rm libmcl.a
