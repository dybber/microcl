#include <stdio.h>
#include <stdlib.h>
#include <mcl.h>

int main()
{
  mclInitialize(3);
  printf("Program ended succesfully.\n");

  // I don't know why this must be here!! Argh!
  cl_program prog;
  clReleaseProgram(prog);


  return 0;
}

