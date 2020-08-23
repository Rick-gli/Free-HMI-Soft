#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include<QThread>
class MyThread : public QThread
{
    Q_OBJECT
public:
   explicit MyThread(QObject *parent);
   void stop();
protected:
   void run() override;
private:
   volatile bool stopped;
};

#endif // MYTHREAD_H
