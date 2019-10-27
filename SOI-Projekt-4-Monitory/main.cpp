 #include <iostream>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include "BufferMonit.h"

using namespace std;
unsigned int timerand(unsigned int rand)
{
    unsigned int time = (rand%2 + 1) * 0;
    return time;
}

void* consumentA(void* monitor)
{
    srand( time(NULL) );

    while(true)
    {
        cout << "ConsumerA wants to read." << endl;
        ((BufferMonit*)monitor)->read('A');

        sleep(timerand(rand()));
    }
}

void* consumentB(void* monitor)
{
    srand( time(NULL) );

    while(true)
    {
        cout << "ConsumerB wants to read." << endl;
        ((BufferMonit*)monitor)->read('B');

        sleep(timerand(rand()));
    }
}

void* consumentC(void* monitor)
{
    srand( time(NULL) );

    while(true)
    {
        cout << "ConsumerC wants to read." << endl;
        ((BufferMonit*)monitor)->read('C');

        sleep(timerand(rand()));
    }
}

void* producerA(void* monitor)
{
    srand( time(NULL) );

    while(true)
    {
        cout << "ProducerA wants to produce." << endl;
        ((BufferMonit*)monitor)->produce(1);

        sleep(timerand(rand()));
    }
}

void* producerB(void* monitor)
{
    srand( time(NULL) );

    while(true)
    {
        cout << "ProducerB wants to produce." << endl;
        ((BufferMonit*)monitor)->produce(2);

        sleep(timerand(rand()));
    }
}

int main()
{
    BufferMonit* mon = new BufferMonit;

    pthread_t ca, cb, cc, pa, pb;

    pthread_create(&ca, NULL, consumentA, (void*) mon);
    pthread_create(&cb, NULL, consumentB, (void*) mon);
    pthread_create(&cc, NULL, consumentC, (void*) mon);
    pthread_create(&pa, NULL, producerA, (void*) mon);
    pthread_create(&pb, NULL, producerB, (void*) mon);

    pthread_join(ca, NULL);
    pthread_join(cb, NULL);
    pthread_join(cc, NULL);
    pthread_join(pa, NULL);
    pthread_join(pb, NULL);

    return 0;
}
