#ifndef MCL_H
#define MCL_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
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
void mclReleaseDeviceData(mclDeviceData* devData);

void mclDataFromDevice(mclContext ctx, mclDeviceData devdata, void* data);

void mclCallKernel(mclContext ctx, cl_program program, char* kernelName, cl_int workSz, mclDeviceData* args, ...);

void mclReleaseContext(mclContext* ctx);


#endif /* MCL_H */
