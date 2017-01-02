#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<lib.h>
#include<minix/type.h>
#include<minix/callnr.h>

int getprocnr(int ident_pid)
{
	message msg;
	msg.m1_i1 = ident_pid;
	return _syscall(0,GETPROCNR,&msg);
}

int main(int argc, char *argv[])
{
	int arg, arg_max, result;
	if(argc < 2)
	{
       printf("Usage: getprocnr arg1 [arg2,arg3,...]\n");
	   return 1;
	}
	arg = atoi(argv[1]);
	arg_max = arg + 10;
	for(;arg<=arg_max;++arg)
	{
		result = getprocnr(arg);
		if(result != -1)
			printf("Indeks procesu o PID %d w tablicy procesowmproc wynosi %d\n", arg, result);
		else
			printf("Nie znaleziono procesu o PID %d. Kod bledu: %d\n", arg, errno);
	}
	
	return 0;
}
