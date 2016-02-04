#include <stdio.h>
#include <stdlib.h>
#include <mcl.h>

int main()
{
  mclContext ctx = mclInitialize(3);
  printf("Program ended succesfully.\n");

  // I don't know why this must be here!! Argh!
  cl_program prog;
  clReleaseProgram(prog);

  mclReleaseContext(&ctx);
  return 0;
}

