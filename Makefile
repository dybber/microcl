OS := $(shell uname -s)
is_64=$(shell s=`uname -m`; if (echo $$s | grep x86_64 > /dev/null); then echo 1; fi)

ifeq ($(OS),Darwin)
    CC=clang
    LIB=-framework OpenCL
    CFLAGS=-Wall -W -O2
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

TEST=vecadd
TESTEXE=$(join $(TEST),.exe)

all: tests/$(TESTEXE)

# AMD
PATH_TO_AMD_INC=/opt/AMDAPP/include/
ifeq ($(is_64), 1)
PATH_TO_AMD_LIB=/opt/AMDAPP/lib/x86_64/
else
PATH_TO_AMD_LIB=/opt/AMDAPP/lib/x86/
endif

# Intel
PATH_TO_INTEL_INC=/usr/include/
ifeq ($(is_64), 1)
PATH_TO_INTEL_LIB=/usr/lib64/
endif

# NVIDIA
PATH_TO_NVIDIA_INC=/usr/local/cuda/include/
PATH_TO_NVIDIA_LIB=/usr/lib/

amd:    $(join $(EXE_NAME),.amd)
apple:  $(join $(EXE_NAME),.apple)
intel:  $(join $(EXE_NAME),.intel)
nvidia: $(join $(EXE_NAME),.nvidia)

%.amd:%.c
	$(CC) $(CFLAGS) -I$(PATH_TO_AMD_INC) -L$(PATH_TO_AMD_LIB) -Wl,-rpath,$(PATH_TO_AMD_LIB)  -o $@ $< -lOpenCL

%.exe: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(LIB) mcl.c $<

%.intel:%.c
	$(CC) $(CFLAGS) -I$(PATH_TO_INTEL_INC) -L$(PATH_TO_INTEL_LIB) -Wl,-rpath,$(PATH_TO_INTEL_LIB) -o $@ $< -lOpenCL

%.nvidia:%.c
	$(CC) $(CFLAGS) -I$(PATH_TO_NVIDIA_INC) -L$(PATH_TO_NVIDIA_LIB) -o $@ $< -lOpenCL

clean:
	rm -f *.amd *.apple *.intel *.nvidia *.o *~ */*~ */*.apple */*.o */*.exe *.exe

TEST=vecadd

.PHONY: test
test: tests/$(TESTEXE)
	(cd tests; ./$(TESTEXE))
