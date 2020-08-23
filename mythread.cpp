#include "mythread.h"
#include "mydatabasedriver.h"

MyThread::MyThread(QObject *parent) :QThread(parent)
{
    stopped=false;
}

void MyThread::stop()
{
    stopped=true;
}

void MyThread::run()
{
    while(!stopped)
    {
        //To do


    }
    stopped=false;
}
