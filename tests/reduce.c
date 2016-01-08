#define _DEBUG

#include <stdio.h>
#include <mcl.h>

// Recursively invoke reduction kernel
void reduce(mclContext ctx,
            cl_kernel kernel, mclDeviceData d_input, mclDeviceData d_output,
            int size, int nThreads, int nBlocks) {
    #ifdef _DEBUG
    printf("  numBlocks: %d\n", nBlocks);
    printf("  numThreads: %d\n", nThreads);
    #endif

    mclSetKernelArg(kernel, 0, sizeof(cl_mem), &d_input.data);
    mclSetKernelArg(kernel, 1, sizeof(cl_mem), &d_output.data);
    mclSetKernelArg(kernel, 2, sizeof(cl_int), &size);
    mclSetKernelArg(kernel, 3, sizeof(cl_int) * nThreads, NULL); /* Shared memory */
    mclInvokeKernel(ctx, kernel, nThreads*nBlocks, nThreads);

    if (nBlocks > 1) {
      int new_nBlocks = (size + nThreads - 1) / nThreads;
      reduce(ctx, kernel, d_output, d_output, nBlocks, nThreads, new_nBlocks);
    }
}

int main() {
    int size = 1<<21;    // number of elements to reduce
    mclContext ctx = mclInitialize(2);

    cl_program p = mclBuildProgram(ctx, "./reduce.cl");

    printf("Creating input array \n");
    int* input = (int*)malloc(size*sizeof(int));
    int expected_out = 0;
    for (int i = 0; i < size; i++) {
      int v = (int)(rand() & 0xFF);
      input[i] = v;
      expected_out += v;
    }

    printf("Copy input data to device memory \n");
    mclDeviceData input_buf = mclDataToDevice(ctx, MCL_RW, MCL_INT, input, size);

    printf("Allocate shared memory \n");

    // For the first iteration only
    int nThreads = 1024; // threads per block
    int nBlocks = (size + nThreads - 1) / nThreads;

    mclDeviceData out_buf = mclAllocDevice(ctx, MCL_RW, MCL_INT, nBlocks);

    cl_kernel kernel = mclCreateKernel(p, "reduce0");

    reduce(ctx, kernel, input_buf, out_buf, size, nThreads, nBlocks);

    cl_int* out = (cl_int*)mclMap(ctx, out_buf, CL_MAP_READ, sizeof(cl_int));
    
    // Check results
    printf("output:   %d\n", *out);
    printf("expected: %d\n", expected_out);

    mclUnmap(ctx, out_buf, out);

    mclReleaseKernel(kernel);
    mclReleaseProgram(p);
    mclReleaseDeviceData(&input_buf);
    mclReleaseDeviceData(&out_buf);
    mclReleaseContext(&ctx);

    // I don't know why this must be here!! Argh!
    clReleaseProgram(NULL);

    return 0;
}
