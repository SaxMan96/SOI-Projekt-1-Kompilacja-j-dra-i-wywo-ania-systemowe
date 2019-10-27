#include <vector>
#include <iostream>
#include "monitor.h"

#define readA cread[0]
#define readB cread[1]
#define readC cread[2]

class BufferMonit : public Monitor
{
    static const int MAX_SIZE = 7;

    std::vector<char> buffer;
    bool cread[3];

    Condition full, full2, empty;
    Condition waitA, waitB, waitC;

public:

    BufferMonit()
    {}

    void showBuffer()
    {
        int i=0;
        std::cout << "\t\t\t\t\t\t\tbuffer:";
        for(std::vector<char>::iterator it = buffer.begin(); it != buffer.end();++i, ++it)
            std::cout << " " << *it;
        for(;i<MAX_SIZE;++i)
            std::cout<<" _";
        std::cout << std::endl;
    }

    char whichProd(int p){
        char litera;
        if(p == 1)
            litera = 'A';
        else
            litera = 'B';
        return litera;
    }
    void produce(int p)
    {
    enter();
        char litera;
        litera = whichProd(p);
        if(litera == 'A')
        {
            if( buffer.size() == MAX_SIZE )
            {
                std::cout << "Producer" << litera << ": buffer full - Producent wait" << std::endl;
                wait(full);
            }

            std::cout << "Producer" << litera << " sets element" << std::endl;
            buffer.push_back(litera+32);
            showBuffer();
        }
        if(litera == 'B')
        {
            if( buffer.size() >= MAX_SIZE -1)
            {
                std::cout << "Producer" << litera << ": buffer full - Producent wait" << std::endl;
                wait(full2);
            }

            std::cout << "Producer" << litera << " sets 2 elements" << std::endl;
            buffer.push_back(litera+32);
            buffer.push_back(litera+32);
            showBuffer();
        }

        if( buffer.size() > 0 )
        {
            std::cout << "Producer" << litera << ": buffer constains 1 element - wake Consumer" << std::endl;
            signal(empty);
        }
    leave();
    }

    void read(char id)
    {
    enter();

        if( buffer.size() == 0 )
        {
            std::cout << "Consumer" << id << ": buffer empty - Consumer read " << std::endl;
            wait(empty);
        }
        if(id == 'A')
        {
            if(readA)
                wait(waitA);
            if(!readA&&readB)
            {
              std::cout << "Consumer" << id << " delete element " << buffer.front() << std::endl;
              buffer.erase(buffer.begin());
              showBuffer();
              readB = 0;
              readC = 0;
              signal(waitB);
              signal(waitC);
            }
            else if(!readA && !readB)
            {
                std::cout << "Consumer" << id << " read element " << buffer.front() << std::endl;
                showBuffer();
                readA = 1;
            }
        }
        else if(id == 'B')
        {
            if(readB)
                wait(waitB);
            if(!readB&&readA)
            {
              std::cout << "Consumer" << id << " delete element " << buffer.front() << std::endl;
              buffer.erase(buffer.begin());
              showBuffer();
              readA = 0;
              readC = 0;
              signal(waitA);
              signal(waitC);
            }
            else if(!readB && !readA)
            {
                std::cout << "Consumer" << id << " read element " << buffer.front() << std::endl;
                showBuffer();
                readB = 1;
            }
        }
        else if(id == 'C')
        {
            if(!readB&&!readA&&!readC)
            {
              std::cout << "Consumer" << id << " delete element " << buffer.front() << std::endl;
              buffer.erase(buffer.begin());
              showBuffer();
              readA = 0;
              readB = 0;
              signal(waitA);
              signal(waitB);
            }
            else if(!readC)
            {
                std::cout << "Consumer" << id << " read element " << buffer.front() << std::endl;
                showBuffer();
                readC = 1;
                wait(waitC);
            }
        }
        if( buffer.size() == MAX_SIZE - 1)
        {
            std::cout << "Consumer" << id << ": buffer is not full - wake Producer " << std::endl;
            signal(full);
        }
        if( buffer.size() == MAX_SIZE - 2)
        {
            std::cout << "Consumer" << id << ": buffer have 2 places - wake Producer " << std::endl;
            signal(full2);
        }
    leave();
    }
};
