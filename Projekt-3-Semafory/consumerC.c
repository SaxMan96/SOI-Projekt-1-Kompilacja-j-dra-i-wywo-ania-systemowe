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

    printf("Consumer C \nCS\n");

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
    else if(!(*readB) && !(*readA) && !(*readC))
    {
      printf("\tRead: %d\n", *readFrom);
      printf("\tRemoved: %d\n", *readFrom);
      --*used;
      *indexr = (*indexr + 1) % SIZE;
      up(empty);
    }
    else if(!(*readC))
    {
      printf("\tRead: %d\n", *readFrom);
      up(full);
      *readC = 1;
    }

    else {
      printf("\tSuspended\n");
      up(full);
    }

    if(*(buffer + ENDPROG) && !(*used))
    {

      ex = 0;
      //for another consuments and this consument
     /* up(full);
      up(full);
      up(full);*/

      printf("\t---Koncze program---\n");
    }

    printf("CS\n");
    printf("Bufor: %d\n", *used);

    if( *readC )
    {
        detach_mem(buffer);
        up(mutex);
        down(rc);
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
