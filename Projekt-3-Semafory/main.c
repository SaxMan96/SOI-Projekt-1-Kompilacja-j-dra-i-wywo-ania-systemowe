#include "shm.h"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sighandler(int a)
{
  remove_sem(MUTEX);
  remove_sem(PRODMUTEX);
  remove_sem(EMPTY);
  remove_sem(FULL);
  remove_sem(RA);
  remove_sem(RB);
  remove_sem(RC);
  remove_sem(WPA);
  remove_sem(WPB);

  remove_mem();

  exit(1);
}

int main(int argc, char **argv)
{
  sem_t *mutex;

  if(argc != 2)
  {
    fprintf(stderr, "Podaj zakres producentow (int)\n");
    return 1;
  }

  //PID procesu dziecka
  pid_t pid;

  //inicjacja pamieci
  init_mem();

  //inicjacja semaforow
  init_sem(MUTEX, 0);
  init_sem(PRODMUTEX,1);
  init_sem(FULL, 0);
  init_sem(EMPTY, SIZE);
  init_sem(RA, 0); // 0 - nie czytal 1 - czytal
  init_sem(RB, 0);
  init_sem(RC, 0);
  init_sem(WPA, 0); // 1 - czeka 0 - nie czeka
  init_sem(WPB, 0);

  signal(SIGINT, sighandler); //zakonczenie w przypadku bledu CTRL + C

  //forkowanie dzieci
  pid = fork();
  if(!pid)
  {
    execvp("./pa", argv);
  }

  pid = fork();
   if(!pid)
  {
    execvp("./pb", argv);
  }

  pid = fork();
  if(!pid)
  {
    execvp("./cc", argv);
  }

  pid = fork();
  if(!pid)
  {
    execvp("./cb", argv);
  }

  pid = fork();
  if(!pid)
  {
    execvp("./ca", argv);
  }

  //begin execution
  printf("\tSTART\n\n");

  mutex = open_sem(MUTEX);
  up(mutex);
  close_sem(mutex);

  //czekam az 5 procesow zakonczy dzialanie
  wait(NULL);
  wait(NULL);
  wait(NULL);
  wait(NULL);
  wait(NULL);

  //odlaczam semafory
  remove_sem(MUTEX);
  remove_sem(PRODMUTEX);
  remove_sem(EMPTY);
  remove_sem(FULL);
  remove_sem(RA);
  remove_sem(RB);
  remove_sem(RC);
  remove_sem(WPA);
  remove_sem(WPB);

  //odlaczam pamiec wspoldzielona
  remove_mem();

  return 0;
}
