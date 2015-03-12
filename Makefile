OS := $(shell uname -s)
is_64=$(shell s=`uname -m`; if (echo $$s | grep x86_64 > /dev/null); then echo 1; fi)

ifeq ($(OS),Darwin)
    CC=clang
    LIB=
    CFLAGS=-Wall -W -O2 -framework OpenCL
    INCLUDES=
else
    CC=gcc
    LIB=-lOpenCL
    CFLAGS=-fopenmp -O2 -std=c99
    OPENCL_ROOTDIR ?= /usr/local/cuda
    OPENCL_LIBDIR  := $(OPENCL_ROOTDIR)/lib64
    OPENCL_INCDIR  ?= $(OPENCL_ROOTDIR)/include
    INCLUDES        = -I$(OPENCL_INCDIR) -I. -I../../include
endif

TESTS=vecadd info
TESTEXES=$(TESTS:%=tests/%.exe)
TESTOUTS=$(TESTS:%=tests/%.out)

.PHONY: test
test: $(TESTOUTS)

.PHONY: exes
exes: $(TESTEXES)

.PHONY: info
info: tests/info.out
	cat $<

.PHONY: clean
clean:
	rm -f *.amd *.apple *.intel *.nvidia *.o *~ */*~ */*.apple */*.o */*.exe *.exe

%.exe: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ mcl.c $< $(LIB)

tests/%.out: tests/%.exe
	(cd tests; ./$*.exe > $*.out)
