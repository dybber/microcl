OS := $(shell uname -s)

ifeq ($(OS),Darwin)
    CC=clang
    LIB=-L..
    CFLAGS=-Wall-W -O2 -framework OpenCL
    INCLUDES=-I..
else
    CC=gcc
    CFLAGS=-Wall -pedantic -Werror -Wshadow  -W -O2 -std=c99
    LIB=-lOpenCL -lmcl
    INCLUDES=-I..
endif

TESTS=vecadd info
TESTEXES=$(TESTS:%=%.exe)
TESTOUTS=$(TESTS:%=%.out)

.PHONY: test
test: $(TESTOUTS)

.PHONY: exes
exes: $(TESTEXES)

# .PHONY: info
# info: info.out
# 	cat $<

.PHONY: clean
clean:
	rm -f *.amd *.apple *.intel *.nvidia *.o *~ */*~ */*.apple */*.o */*.exe *.exe *.out

%.exe: %.c
	$(CC) $(CFLAGS) -L.. $(INCLUDES) -o $@ $< $(LIB)

%.out: %.exe
	(./$*.exe > $*.out)
