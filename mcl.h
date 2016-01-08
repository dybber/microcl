#ifndef MCL_H
#define MCL_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

/* Sligtly modified version of the same in CLU */
#ifdef _DEBUG
#define MCL_VALIDATE(in_status, msg)                \
{\
    if (in_status != CL_SUCCESS)\
    {\
      printf("%s(%d): %s. OpenCL returned: %d (%s)\n", \
               __FILE__, __LINE__, msg, in_status, mclErrorToString(in_status)); \
    }\
}
#else
#define MCL_VALIDATE(in_status, msg) ((void) in_status)
#endif

typedef struct
{
  cl_device_id     device_id;
  cl_context       context;
  cl_command_queue command_queue;
} mclContext;

typedef unsigned int mclType;
#define MCL_FLOAT 0
#define MCL_DOUBLE 1
#define MCL_INT 2

typedef cl_int mclBufType;
#define MCL_RW CL_MEM_READ_WRITE
#define MCL_R CL_MEM_READ
#define MCL_W CL_MEM_WRITE

typedef struct
{
  mclBufType  buftype;
  mclType     type;
  cl_mem      data;
  cl_int      n;
} mclDeviceData;

mclContext mclInitialize(unsigned int log_level);
cl_program mclBuildProgram(mclContext ctx, const char* filename);

mclDeviceData mclDataToDevice(mclContext ctx, mclBufType buftype, mclType type, void* data, cl_int n);

void* mclMap(mclContext ctx, mclDeviceData dd, cl_map_flags flags, size_t sz);
void mclUnmap(mclContext ctx, mclDeviceData dd, void* ptr);

mclDeviceData mclAllocDevice(mclContext ctx, mclBufType buftype, mclType type, cl_int n);

void mclReleaseDeviceData(mclDeviceData* devData);

void mclDataFromDevice(mclContext ctx, mclDeviceData devdata, void* data);

void mclCallKernel(mclContext ctx, cl_program program, char* kernelName, cl_int workSz, mclDeviceData* args, ...);

cl_kernel mclCreateKernel(cl_program program, char* kernelName);
void mclSetKernelArg(cl_kernel kernel, cl_uint i, 
                     size_t arg_size, const void *arg_value);
void mclInvokeKernel(mclContext ctx, cl_kernel kernel,
                     cl_uint global_work_size, cl_uint local_work_size);
void mclReleaseKernel(cl_kernel kernel);

void mclReleaseContext(mclContext* ctx);
void mclReleaseProgram(cl_program prog);

const char* mclErrorToString(cl_int errcode_ret);

#endif /* MCL_H */
