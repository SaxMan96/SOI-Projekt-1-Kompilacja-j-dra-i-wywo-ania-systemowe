#include "shm.h"
#include <limits.h>

int main(int argc, char **argv)
{
  int m_fd, value, ex, infinite;
  int *buffer, *used, *indexw, *writeTo;

  sem_t *mutex, *prodmutex, *empty, *full, *waitpa, *waitpb;

  value = 1;
  ex = atoi(*(argv + 1));

  if(!ex) {
  	ex = 1;
  	infinite = 1;
  }
  else
  	infinite = 0;

  prodmutex = open_sem(PRODMUTEX);
  mutex = open_sem(MUTEX);
  empty = open_sem(EMPTY);
  full = open_sem(FULL);
  waitpa = open_sem(WPA);
  waitpb = open_sem(WPB);

  m_fd = get_mem();

  srand(time(NULL));

  while(ex--)
  {
  	if(infinite)
  		++ex;

    down(prodmutex);
    down(empty);
    down(empty);
    down(mutex);

    printf("\tProducerB \nCS\n");

    buffer = attach_mem(m_fd);

    //dodawanie pierwszego elementu
        used = buffer + USED;
        indexw = buffer + INDEXW;
        writeTo = buffer + *indexw;
        *writeTo = value;
        *indexw = ( *indexw + 1 ) % SIZE;
        ++*used;

    //dodawanie drugiego elementu
        used = buffer + USED;
        indexw = buffer + INDEXW;
        writeTo = buffer + *indexw;
        *writeTo = value;
        *indexw = ( *indexw + 1 ) % SIZE;
        ++*used;

    value %= INT_MAX;
    ++value;

    printf("\tWrite:%d, %d\n", *writeTo, *writeTo);

    if(!ex)
    {
      printf("\t---Koncze program---\n");
      *(buffer + ENDPROG) = 1;
    }



    printf("CS\n");
    printf("Bufor: %d\n", *used);

detach_mem(buffer);
    up(full);
    up(full);
    up(mutex);
    up(prodmutex);
    up(waitpa);
    down(waitpb);
    //simulate data processing
     sleep( 1 );
  }

  close_sem(mutex);
  close_sem(empty);
  close_sem(full);
  close_sem(prodmutex);
  close_sem(waitpa);
  close_sem(waitpb);

  return 0;
}
