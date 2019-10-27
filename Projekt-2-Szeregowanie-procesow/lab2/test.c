#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <lib.h>
#include <minix/type.h>

int main(int argc, char* argv[])
{
  message msg;
  msg.m1_i1 = getpid();
  msg.m1_i2 = atoi(argv[1]);
  _syscall(0,69, &msg);
  while(1){}
  return 0;
}
