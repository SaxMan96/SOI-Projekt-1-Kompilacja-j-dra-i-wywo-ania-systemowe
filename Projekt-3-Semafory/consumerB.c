#include "shm.h"

int main()
{
  int m_fd, ex;
  int *buffer, *used,
      *readA, *readB, *readC,
      *indexr, *readFrom;

  sem_t *mutex, *empty, *full, *ra, *rb, *rc;

  mutex = open_sem(MUTEX);
  empty = open_sem(EMPTY);
  full = open_sem(FULL);
  ra = open_sem(RA);
  rb = open_sem(RB);
  rc = open_sem(RC);

  m_fd = get_mem();

  ex = 1;

  srand(time(NULL));

  while(ex)
  {
    down(full);
    down(mutex);

    printf("Consumer B\nCS\n");

    buffer = attach_mem(m_fd);

    //set helper pointers
    used = buffer + USED;

    readA = buffer + RDA;
    readB = buffer + RDB;
    readC = buffer + RDC;
/*
    waitA = buffer + WTA;
    waitB = buffer + WTB;
    waitC = buffer + WTC;
*/
    indexr = buffer + INDEXR;
    readFrom = buffer + *indexr;


    if(*(buffer + ENDPROG) && !(*used)) {}
    else if(!(*readB)&&(*readA))
    {
      printf("\tRead: %d\n", *readFrom);
      printf("\tRemoved: %d\n", *readFrom);
      --*used;
      *indexr = (*indexr + 1) % SIZE;

      *readA = 0; up(ra);  //readA
      if( *readC ) { *readC = 0; up(rc); } //readC
      *readB = 0;  //readB

      up(empty);
    }
    else if(!(*readB)&&!(*readA))
    {
       printf("\tRead: %d\n", *readFrom);
      *readB=1;
      up(full);
    }

    else if(*readB)
    {
      printf("\tSuspended\n");
      up(full);
    }

    if(*(buffer + ENDPROG) && !(*used))
    {
      ex = 0;
      //for another consuments and this consument
      //up(full);
      //up(full);
      //up(full);

      printf("\t---Koncze program---\n");
    }

    printf("CS\n");
    printf("Bufor: %d\n", *used);

    if( *readB )
    {
        detach_mem(buffer);
        up(mutex);
        down(rb);
    }
    else
    {
        detach_mem(buffer);
        up(mutex);
    }

    sleep( 2 );
  }//while

  close_sem(mutex);
  close_sem(empty);
  close_sem(full);
  close_sem(ra);
  close_sem(rb);
  close_sem(rc);

  return 0;
}
