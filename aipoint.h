#ifndef AIPOINT_H
#define AIPOINT_H

#include <QObject>
#include <QWidget>
#include<QLabel>
#include"DataType.h"
class AIpoint : public QWidget
{
    Q_OBJECT
public:
    explicit AIpoint(QWidget *parent = nullptr);
private:
    const char* tagname;
    QLabel *label=nullptr;
    unsigned int registerindex;
    float ENG_MAX=100.0;
    float ENG_MIN=0.0;
    float realValue=0.0;
    float HH=0.0;
    float HI=0.0;
    float LO=0.0;
    float LL=0.0;
    unsigned int pageIndex=0;
    unsigned int currentDisplaypage=0;
    unsigned int alarmMask=0;
    /*
        0-LL ENABLE
        1-L  ENABLE
        2-H  ENABLE
        3-HH ENABLE
        4-LL HAS ALARM
        5-L  HAS ALARM
        6-H  HAS ALARM
        7-HH HAS ALARM
    */

public:
    void bingParameter(QLabel *L,unsigned int rindex,const char * tagname, float engmax=100,float engmin=0,float halarm=0,float loalarm=0);
    void getRealValue();

private slots:
    void updateRealValue();

};

#endif // AIPOINT_H
