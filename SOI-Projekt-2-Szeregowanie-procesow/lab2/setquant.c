#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <lib.h>
#include <minix/type.h>

int main(int argc, char* argv[])
{
  message msg;
  if(argc != 3)
  {
    printf("Usage: ./setq group ticks\n");
    return -1;
  }
  msg.m1_i1 = atoi(argv[2]);
  msg.m1_i2 = atoi(argv[1]);
  _syscall(MM,SETQUANT,&msg);

  if(atoi(argv[1]) < 0 || atoi(argv[1]) > 2 || atoi(argv[2]) < 0){
	printf("Wrong usage of program!\n");
	return -1;
  }
  printf("Quant of group: ");
  switch(atoi(argv[1]))
  {
     case 0:
	printf("A ");
	break;
     case 1:
	printf("B ");
	break;
     case 2:
	printf("C ");
	break;
  }
  printf("set as: %d\n", atoi(argv[2]));
  return 1;
}
