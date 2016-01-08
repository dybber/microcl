#include "mcl.h"

#define MCL_MAX_STRING_LENGTH 1024
#define MCL_MAX_SOURCE_SIZE (0x100000)

#include "mcl_device_info.h"

#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

static unsigned int mclLogLevel = 0;

void logStream(FILE* os, const char* fmt, ...)
{
  char buffer[30];
  struct timeval tv;
  time_t curtime;
  gettimeofday(&tv, NULL); 
  curtime=tv.tv_sec;
  strftime(buffer,30,"%Y-%m-%d %T.",localtime(&curtime));
  fprintf(os, "MCL %s%d - ", buffer, (int)tv.tv_usec);
  va_list args;
  va_start(args,fmt);
  vfprintf(os,fmt,args);
  va_end(args);
  fprintf(os, "\n");
}

void logOclCall(const char* fmt, ...)
{
  if (mclLogLevel > 0) {
    va_list args;
    va_start(args,fmt);
    logStream(stdout,fmt,args);
    va_end(args);
  }
}

/* Taken from CLU */
#define STRINGIFY(x) #x
#define CASE_STRINGIFY(X) case X : return #X;

const char* mclErrorToString(cl_int errcode_ret)
{
    switch (errcode_ret)
    {
        CASE_STRINGIFY(CL_SUCCESS);
        CASE_STRINGIFY(CL_DEVICE_NOT_FOUND);
        CASE_STRINGIFY(CL_DEVICE_NOT_AVAILABLE);
        CASE_STRINGIFY(CL_COMPILER_NOT_AVAILABLE);
        CASE_STRINGIFY(CL_MEM_OBJECT_ALLOCATION_FAILURE);
        CASE_STRINGIFY(CL_OUT_OF_RESOURCES);
        CASE_STRINGIFY(CL_OUT_OF_HOST_MEMORY);
        CASE_STRINGIFY(CL_PROFILING_INFO_NOT_AVAILABLE);
        CASE_STRINGIFY(CL_MEM_COPY_OVERLAP);
        CASE_STRINGIFY(CL_IMAGE_FORMAT_MISMATCH);
        CASE_STRINGIFY(CL_IMAGE_FORMAT_NOT_SUPPORTED);
        CASE_STRINGIFY(CL_BUILD_PROGRAM_FAILURE);
        CASE_STRINGIFY(CL_MAP_FAILURE);
        CASE_STRINGIFY(CL_INVALID_VALUE);
        CASE_STRINGIFY(CL_INVALID_DEVICE_TYPE);
        CASE_STRINGIFY(CL_INVALID_PLATFORM);
        CASE_STRINGIFY(CL_INVALID_DEVICE);
        CASE_STRINGIFY(CL_INVALID_CONTEXT);
        CASE_STRINGIFY(CL_INVALID_QUEUE_PROPERTIES);
        CASE_STRINGIFY(CL_INVALID_COMMAND_QUEUE);
        CASE_STRINGIFY(CL_INVALID_HOST_PTR);
        CASE_STRINGIFY(CL_INVALID_MEM_OBJECT);
        CASE_STRINGIFY(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
        CASE_STRINGIFY(CL_INVALID_IMAGE_SIZE);
        CASE_STRINGIFY(CL_INVALID_SAMPLER);
        CASE_STRINGIFY(CL_INVALID_BINARY);
        CASE_STRINGIFY(CL_INVALID_BUILD_OPTIONS);
        CASE_STRINGIFY(CL_INVALID_PROGRAM);
        CASE_STRINGIFY(CL_INVALID_PROGRAM_EXECUTABLE);
        CASE_STRINGIFY(CL_INVALID_KERNEL_NAME);
        CASE_STRINGIFY(CL_INVALID_KERNEL_DEFINITION);
        CASE_STRINGIFY(CL_INVALID_KERNEL);
        CASE_STRINGIFY(CL_INVALID_ARG_INDEX);
        CASE_STRINGIFY(CL_INVALID_ARG_VALUE);
        CASE_STRINGIFY(CL_INVALID_ARG_SIZE);
        CASE_STRINGIFY(CL_INVALID_KERNEL_ARGS);
        CASE_STRINGIFY(CL_INVALID_WORK_DIMENSION);
        CASE_STRINGIFY(CL_INVALID_WORK_GROUP_SIZE);
        CASE_STRINGIFY(CL_INVALID_WORK_ITEM_SIZE);
        CASE_STRINGIFY(CL_INVALID_GLOBAL_OFFSET);
        CASE_STRINGIFY(CL_INVALID_EVENT_WAIT_LIST);
        CASE_STRINGIFY(CL_INVALID_EVENT);
        CASE_STRINGIFY(CL_INVALID_OPERATION);
        CASE_STRINGIFY(CL_INVALID_GL_OBJECT);
        CASE_STRINGIFY(CL_INVALID_BUFFER_SIZE);
        CASE_STRINGIFY(CL_INVALID_MIP_LEVEL);
        CASE_STRINGIFY(CL_INVALID_GLOBAL_WORK_SIZE);
#ifdef CL_VERSION_1_2
        CASE_STRINGIFY(CL_INVALID_PROPERTY);
        CASE_STRINGIFY(CL_INVALID_IMAGE_DESCRIPTOR);
        CASE_STRINGIFY(CL_INVALID_COMPILER_OPTIONS);
        CASE_STRINGIFY(CL_INVALID_LINKER_OPTIONS);
        CASE_STRINGIFY(CL_INVALID_DEVICE_PARTITION_COUNT);
#endif
    }
    return "Unknown CL error";
}


cl_platform_id mclGetPlatformID()
{
  /* Get the platform information */
  cl_uint num_platforms;
  clGetPlatformIDs(0, NULL, &num_platforms);             // get number of platforms
  cl_platform_id* platforms = calloc(sizeof(cl_platform_id), num_platforms);
  clGetPlatformIDs(num_platforms, platforms, NULL);     // populate platform table
  if (mclLogLevel > 1) {
    logStream(stdout,"System has %d platform(s):", num_platforms);
    for(cl_uint i = 0; i < num_platforms;i++) {
      char buf[MCL_MAX_STRING_LENGTH];
      clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, MCL_MAX_STRING_LENGTH, buf, NULL);
      logStream(stdout,"  Platform: %s", buf);
    }
  }
  if (num_platforms == 0) {
    fprintf(stderr, "MCL - Exiting: No OpenCL Platform available\n");
    exit(1);
  }
  return platforms[0];
}

cl_device_id mclGetGpuDeviceID()
{
  cl_platform_id platform_id = mclGetPlatformID();

  // Info about devices
  cl_uint num_devices;
  clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
  cl_device_id* devices = calloc(sizeof(cl_device_id), num_devices);
  clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);

  if (mclLogLevel > 1) {
    logStream(stdout, "System has %d GPU device(s):", num_devices);
    char buf1[MCL_MAX_STRING_LENGTH];
    char buf2[MCL_MAX_STRING_LENGTH];
    for(cl_uint i = 0; i < num_devices;i++) {
      clGetDeviceInfo(devices[i], CL_DEVICE_NAME, MCL_MAX_STRING_LENGTH, buf1, NULL);
      clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, MCL_MAX_STRING_LENGTH, buf2, NULL);
      logStream(stdout, "   Device %d: %s, which supports %s", i, buf1, buf2);
    }
  }

  if (num_devices < 1) {
    fprintf(stderr, "MCL - Exiting: No GPU devices available\n");
    exit(1);
  }

  // Selecting the device
  cl_uint num_device = num_devices - 1;
  cl_device_id device_id = devices[num_device];
  if (mclLogLevel > 0) {
    char buf[MCL_MAX_STRING_LENGTH];
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, MCL_MAX_STRING_LENGTH, buf, NULL);
    logStream(stdout,"Selected device %d: %s", num_device, buf);
    if (mclLogLevel > 2) {
      mclPrintDeviceInfo(device_id);
    }
  }
  return device_id;
}

cl_device_id mclGetBestDeviceID()
{
  cl_platform_id platform_id = mclGetPlatformID();

  // Info about devices
  cl_uint num_devices;
  clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
  cl_device_id* devices = calloc(sizeof(cl_device_id), num_devices);
  clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);

  if (mclLogLevel > 1) {
    logStream(stdout, "System has %d device(s):", num_devices);
    char buf1[MCL_MAX_STRING_LENGTH];
    char buf2[MCL_MAX_STRING_LENGTH];
    for(cl_uint i = 0; i < num_devices;i++) {
      clGetDeviceInfo(devices[i], CL_DEVICE_NAME, MCL_MAX_STRING_LENGTH, buf1, NULL);
      clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, MCL_MAX_STRING_LENGTH, buf2, NULL);
      logStream(stdout, "   Device %d: %s, which supports %s", i, buf1, buf2);
    }
  }

  if (num_devices < 1) {
    fprintf(stderr, "MCL - Exiting: No devices available\n");
    exit(1);
  }

  // Selecting the device
  cl_uint num_device = 0;
  cl_uint bestIndex = 0;
  for (cl_uint i = 0; i < num_devices; i++) {
    cl_uint maxComputeUnits, maxClockFrequency;
    clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &maxComputeUnits, NULL);
    clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &maxClockFrequency, NULL);
    cl_uint index = maxComputeUnits * maxClockFrequency;
    if (index > bestIndex) {
      bestIndex = index;
      num_device = i;
    }
  }

  cl_device_id device_id = devices[num_device];
  if (mclLogLevel > 0) {
    char buf[MCL_MAX_STRING_LENGTH];
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, MCL_MAX_STRING_LENGTH, buf, NULL);
    logStream(stdout,"Selected device %d: %s", num_device, buf);
    if (mclLogLevel > 2) {
      mclPrintDeviceInfo(device_id);
    }
  }
  return device_id;
}


cl_context mclCreateContext(cl_device_id* device_id) 
{
  cl_int ret;
  logOclCall("clCreateContext");
  cl_context context = clCreateContext( NULL, 1, device_id, NULL, NULL, &ret);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Exiting: Failed to create context\n");
    exit(1);
  }
  return context;
}

cl_command_queue mclCreateCommandQueue(cl_context context, cl_device_id device_id)
{
  cl_int ret;
  logOclCall("clCreateCommmandQueue");
  cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Exiting: Failed to create command queue\n");
    exit(1);
  }
  return command_queue;
}

mclContext mclInitialize(unsigned int log_level) {
  mclLogLevel = log_level;
  mclContext context;
  context.device_id = mclGetBestDeviceID();
  context.context = mclCreateContext(&context.device_id);
  context.command_queue = mclCreateCommandQueue(context.context,context.device_id);
  return context;
}

cl_program mclBuildProgram(mclContext ctx, const char* filename)
{
  cl_device_id device_id = ctx.device_id;
  cl_context context = ctx.context;
  // Read file
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "MCL - Exiting: Failed to load kernel.\n");
    exit(1);
  }
  char *source_str = (char *)malloc(MCL_MAX_STRING_LENGTH);
  size_t source_size = fread( source_str, 1, MCL_MAX_STRING_LENGTH, fp );
  fclose( fp );

  cl_int ret;
  logOclCall("clCreateProgramWithSource");
  cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Exiting: Failed to create program.\n");
    exit(1);
  } 
  logOclCall("clBuildProgram");
  ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Exiting: Failed to build program.\n");
    char *build_log;
    size_t ret_val_size;
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);
    build_log = (char*) malloc(sizeof(char)*(ret_val_size+1));
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);
    // terminate with \0; the reference says nothing about whether the string is 0 terminated
    build_log[ret_val_size] = '\0';
    fprintf(stderr, "MCL - Error in clBuildProgram, Line %u in file %s\n", __LINE__, __FILE__);
    fprintf(stderr, "MCL - Error in kernel file %s:\n%s\n", filename, build_log);
    free (build_log);
    exit(1);
  }
  free(source_str);
  return program;
}


inline size_t mclSizeOfType(mclType t) {
  size_t sz;
  if (t == MCL_FLOAT) {
    sz = sizeof(float);
  } else if (t == MCL_INT) {
    sz = sizeof(int);
  } else if (t == MCL_DOUBLE) {
    sz = sizeof(double);
  } else {
    fprintf(stderr, "MCL - Exiting: Unknown type of data.\n");
    exit(1);
  }     
  return sz;
}

mclDeviceData mclDataToDevice(mclContext ctx, mclBufType buftype, mclType type, void* data, cl_int n)
{
  // Copy argument to device    
  cl_int ret;
  size_t sz = n * mclSizeOfType(type);
  logOclCall("clCreateBuffer");
  cl_mem data_device = clCreateBuffer(ctx.context, buftype, sz, NULL, &ret);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Error creating buffer on device.\n");
    exit(1);
  }
  logOclCall("clEnqueueWriteBuffer");
  ret = clEnqueueWriteBuffer(ctx.command_queue, data_device, CL_TRUE, 0, sz, data, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Error copying data to device.\n");
    exit(1);
  }
  mclDeviceData res;
  res.buftype = buftype;
  res.type = type;
  res.data = data_device;
  res.n = n;
  return res;
}

void* mclMap(mclContext ctx, mclDeviceData dd, cl_map_flags flags, size_t sz)
{
  logOclCall("clEnqueueMapBuffer");
  cl_int ret;
  cl_int* out = (cl_int*)clEnqueueMapBuffer(ctx.command_queue, dd.data, CL_TRUE, flags, 
                                            0, sz, 0, 0, 0, &ret);
  MCL_VALIDATE(ret, "Error mapping data");
  return out;
}

void mclUnmap(mclContext ctx, mclDeviceData dd, void* ptr)
{
  logOclCall("clEnqueueUnmapMemObject");
  cl_int ret;
  ret = clEnqueueUnmapMemObject(ctx.command_queue, dd.data, ptr, 0, 0, 0);
  MCL_VALIDATE(ret, "Error unmapping data");
}

mclDeviceData mclAllocDevice(mclContext ctx, mclBufType buftype, mclType type, cl_int n)
{
  // Copy argument to device    
  cl_int ret;
  size_t sz = n * mclSizeOfType(type);
  logOclCall("clCreateBuffer");
  cl_mem data_device = clCreateBuffer(ctx.context, buftype, sz, NULL, &ret);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Error creating buffer on device.\n");
    exit(1);
  }
  mclDeviceData res;
  res.buftype = buftype;
  res.type = type;
  res.data = data_device;
  res.n = n;
  return res;
}

void mclReleaseDeviceData(mclDeviceData* devData)
{
  cl_int ret;
  logOclCall("clReleaseMemObject");
  ret = clReleaseMemObject(devData->data);
  devData->data = NULL;
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Failed to release device data.\n");
    exit(1);
  }
}
  

//void mclDataFromDevice(mclContext ctx, mclDeviceData devdata, void* data);

void mclCallKernel(mclContext ctx, cl_program program, char* kernelName, cl_int workSz, mclDeviceData* args, ...)
{
  // Create the kernel
  cl_int ret;
  logOclCall("clCreateKernel");
  cl_kernel kernel = clCreateKernel(program, kernelName, &ret);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Error creating kernel '%s' from program.\n", kernelName);
    exit(1);
  }
  // Attach arguments
  va_list ap;
  va_start(ap, args);
  mclDeviceData* devData = args;
  for (int i = 0; devData != NULL; i++) {
      logOclCall("clSetKernelArg");
      ret = clSetKernelArg(kernel, i, sizeof(cl_mem), (void*)&((*devData).data));

      if (ret != CL_SUCCESS) {
        fprintf(stderr, "MCL - Error setting argument %d to kernel %s.\n", i, kernelName);
        exit(1);
      }
      devData = va_arg(ap,mclDeviceData*);
    }
  va_end(ap);
  // Do the call
  size_t global_work_size[3] = {workSz};
  logOclCall("clEnqueueNDRangeKernel");
  ret = clEnqueueNDRangeKernel(ctx.command_queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Error calling kernel %s.\n", kernelName);
    exit(1);
  }
  ret = clReleaseKernel(kernel);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Error releasing kernel %s.\n", kernelName);
    exit(1);
  }
}

cl_kernel mclCreateKernel(cl_program program, char* kernelName)
{
  // Create the kernel
  cl_int ret;
  logOclCall("clCreateKernel");
  cl_kernel kernel = clCreateKernel(program, kernelName, &ret);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Error creating kernel '%s' from program.\n", kernelName);
    exit(1);
  }
  return kernel;
}

void mclSetKernelArg(cl_kernel kernel, cl_uint i, 
                     size_t arg_size, const void *arg_value)
{
  cl_int ret;
  logOclCall("clSetKernelArg");
  ret = clSetKernelArg(kernel, i, arg_size, arg_value);
  MCL_VALIDATE(ret, "Error setting argument");
}

void mclInvokeKernel(mclContext ctx, cl_kernel kernel,
                     cl_uint global_work_size, cl_uint local_work_size) {
  cl_int ret;
  logOclCall("clEnqueueNDRangeKernel");
  size_t global_ws[3] = {global_work_size};
  size_t local_ws[3] = {local_work_size};
  ret = clEnqueueNDRangeKernel(ctx.command_queue, kernel, 1, 
                               NULL, global_ws, local_ws,
                               0, NULL, NULL);
  MCL_VALIDATE(ret, "Error invoking kernel");
}

void mclReleaseKernel(cl_kernel kernel)
{
  logOclCall("clReleaseKernel");
  cl_int ret = clReleaseKernel(kernel);
  MCL_VALIDATE(ret, "Error releasing kernel");
}

void mclReleaseContext(mclContext* ctx)
{
  cl_int ret;
  ret = clFlush(ctx->command_queue);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Failing to flush command queue.\n");
    exit(1);
  }
  ret = clFinish(ctx->command_queue);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Failing to finish command queue.\n");
    exit(1);
  }
  ret = clReleaseCommandQueue(ctx->command_queue);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Failing releasing command queue.\n");
    exit(1);
  }
  ctx->command_queue = NULL;

  ret = clReleaseContext(ctx->context);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Failing releasing context.\n");
    exit(1);
  }
  ctx->context = NULL;
  ctx->device_id = NULL;
}

void mclReleaseProgram(cl_program prog)
{
  logOclCall("clReleaseProgram");
  cl_int ret = clReleaseProgram(prog);
  MCL_VALIDATE(ret, "Failed to release program");
}
