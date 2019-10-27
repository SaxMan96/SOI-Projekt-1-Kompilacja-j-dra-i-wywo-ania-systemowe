#include <pthread.h>

#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

#include <time.h>

//defined constants
#define MNAME   "/memo"
#define SIZE    (50)
#define RWU     S_IRUSR | S_IWUSR
#define RWG     S_IRGRP | S_IWGRP
#define RWO     S_IROTH | S_IWOTH
#define MOD     RWU
#define MILNAN  (1000000)
#define MICNAN  (1000)

//special buffer indexes
#define USED      SIZE
#define INDEXR    (SIZE + 1)
#define INDEXW    (SIZE + 2)
#define RDA       (SIZE + 3)
#define RDB       (SIZE + 4)
#define RDC       (SIZE + 5)
#define WTPA        (SIZE + 6)
#define WTPB        (SIZE + 7)
#define ENDPROG   (SIZE + 8)
#define BUFFSIZE  (SIZE + 9)

//semaphores
#define MUTEX        "/mutex"
#define PRODMUTEX    "/prodmutex"
#define EMPTY        "/empty"
#define FULL         "/full"
#define RA           "/ra"
#define RB           "/rb"
#define RC           "/rc"
#define WPA          "/wpa"
#define WPB          "/wpb"

void nsleep(long nsec)
{
  struct timespec tim;

  tim.tv_sec = 0;
  tim.tv_nsec = nsec;

  nanosleep(&tim, NULL);
}

sem_t* open_sem(const char* name)
{
  sem_t *ret;

  ret = sem_open(name, O_RDWR);

  if(ret == SEM_FAILED)
  {
    perror("open_sem");
    exit(1);
  }

  return ret;
}

void close_sem(sem_t *sem)
{
  if( sem_close(sem) == -1 )
  {
    perror("close_sem");

    exit(1);
  }
}

void init_sem(const char* name, unsigned int value)
{
  sem_t* ret;

  ret = sem_open(name, O_CREAT | O_EXCL, MOD, value);

  if(ret == SEM_FAILED)
  {
    perror("init_sem");

    exit(1);
  }

  sem_close(ret);
}

int* attach_mem(int);
void detach_mem(int*);

void init_mem()
{
  int ret;
  int* buffer;

  ret = shm_open(MNAME, O_CREAT | O_EXCL | O_RDWR, MOD);

  if(ret == -1)
  {
    perror("init_mem");
    exit(1);
  }

  ftruncate(ret, BUFFSIZE * sizeof(int));

  buffer = attach_mem(ret);

  //initialize values
  *(buffer + USED) =    0; // ilosc elementow w buforze
  *(buffer + INDEXR) =  0; // indeks do czytania
  *(buffer + INDEXW) =  0; // indeks do zapisu
  *(buffer + RDA) =     0; //0 - nie przeczytal 1 - przeczytal
  *(buffer + RDB) =     0;
  *(buffer + RDC) =     0;
  *(buffer + WTPA) =    0; //0 - nie czeka 1 - czeka
  *(buffer + WTPB) =    0;
  *(buffer + ENDPROG) = 0;

  detach_mem(buffer);
}

int get_mem()
{
  int ret;

  ret = shm_open(MNAME, O_RDWR, 0);

  if(ret == -1)
  {
    perror("get_mem");
    exit(1);
  }

  return ret;
}

int* attach_mem(int shmfd)
{
  int* ret;

  ret = mmap(NULL, BUFFSIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0); //mapuje pamiec wspuldzielona na wlasciwa przestrzeen adresowa

  if(ret == MAP_FAILED)
  {
    perror("attach_mem");
    exit(1);
  }

  return ret;
}

void detach_mem(int* shmaddr)
{
  if( munmap(shmaddr, BUFFSIZE * sizeof(int)) == -1 )
  {
    perror("detach_mem");
    exit(1);
  }
}

void remove_sem(const char* name)
{
  if( sem_unlink(name) == -1 )
  {
    perror("remove_sem");

    exit(1);
  }
}

void remove_mem()
{
  if( shm_unlink(MNAME) == -1 )
  {
    perror("remove_mem");

    exit(1);
  }
}

void down(sem_t *sem)
{
  if( sem_wait(sem) == -1 )
  {
    perror("down");

    exit(1);
  }
}

void up(sem_t *sem)
{
  if( sem_post(sem) == -1 )
  {
    perror("up");

    exit(1);
  }
}
