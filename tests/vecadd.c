#include <stdio.h>
#include <stdlib.h>
#include "../mcl.h"

int main()
{
  mclContext ctx = mclInitialize(2);

  cl_program program = mclBuildProgram(ctx, "./kernel.cl");

  cl_int arg1_sz = 343;
  float arg1[arg1_sz];
  for(cl_int i = 0; i < arg1_sz; i++ ) {
    arg1[i] = (float)i;
  }

  // Copy argument to GPU    
  mclDeviceData dev_data = mclDataToDevice(ctx, MCL_RW, MCL_FLOAT, arg1, arg1_sz);

  // Call Kernel Program with argument
  mclCallKernel(ctx, program, "vecAdd14", arg1_sz, &dev_data, (mclDeviceData*)NULL);

  // Copy result back to host
  cl_int ret;
  ret = clEnqueueReadBuffer(ctx.command_queue, dev_data.data, CL_TRUE, 0, arg1_sz * sizeof(float), arg1, 0, NULL, NULL);
  if (ret != CL_SUCCESS) {
    fprintf(stderr, "MCL - Error copying GPU data to host\n");
    exit(1);
  }

  // Print result
  for(cl_int i=0; i<arg1_sz; i++) {
    printf("arg1[%d] : %f\n", i, arg1[i]);
  }
  
  ret = clReleaseProgram(program);
  mclReleaseDeviceData(&dev_data);
  mclReleaseContext(&ctx);
  
  return 0;
}

