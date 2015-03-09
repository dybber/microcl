UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    SYSTEM=apple
else
    SYSTEM=intel
endif

all: $(SYSTEM)

is_64=$(shell s=`uname -m`; if (echo $$s | grep x86_64 > /dev/null); then echo 1; fi)

CFLAGS=-Wall -W -O2

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

EXE_NAME=tests/vecadd

CC=clang
#CC=gcc

amd:    $(join $(EXE_NAME),.amd)
apple:  $(join $(EXE_NAME),.apple)
intel:  $(join $(EXE_NAME),.intel)
nvidia: $(join $(EXE_NAME),.nvidia)

%.amd:%.c
	$(CC) $(CFLAGS) -I$(PATH_TO_AMD_INC) -L$(PATH_TO_AMD_LIB) -Wl,-rpath,$(PATH_TO_AMD_LIB)  -o $@ $< -lOpenCL

%.apple: %.c
	$(CC) $(CFLAGS) -framework OpenCL -o $@ mcl.c $^

%.intel:%.c
	$(CC) $(CFLAGS) -I$(PATH_TO_INTEL_INC) -L$(PATH_TO_INTEL_LIB) -Wl,-rpath,$(PATH_TO_INTEL_LIB) -o $@ $< -lOpenCL

%.nvidia:%.c
	$(CC) $(CFLAGS) -I$(PATH_TO_NVIDIA_INC) -L$(PATH_TO_NVIDIA_LIB) -o $@ $< -lOpenCL

clean:
	rm -f *.amd *.apple *.intel *.nvidia *.o *~ */*~ */*.apple */*.o

TEST=vecadd

.PHONY: test
test: tests/$(TEST).$(SYSTEM)
	(cd tests; ./$(TEST).$(SYSTEM))
