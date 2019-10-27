#include "shm.h"

int main()
{

  sem_unlink(MUTEX);
  sem_unlink(PRODMUTEX);
  sem_unlink(EMPTY);
  sem_unlink(FULL);
  sem_unlink(RA);
  sem_unlink(RB);
  sem_unlink(RC);
  /*
  sem_unlink(WA);
  sem_unlink(WB);
  sem_unlink(WC);
  */
  sem_unlink(WPA);
  sem_unlink(WPB);

  shm_unlink(MNAME);
  remove_mem();

  return 0;
}

