#include "aipoint.h"
#include <QTimer>
AIpoint::AIpoint(QWidget *parent) : QWidget(parent)
{

}

void AIpoint::bingParameter(QLabel *L, unsigned int rindex, const char *tagname, float engmax, float engmin, float halarm, float loalarm)
{
    this->label=L;
    this->registerindex=rindex;
    this->ENG_MAX=engmax;
    this->ENG_MIN=engmin;
    this->HI=halarm;
    this->LO=loalarm;
    this->tagname=tagname;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateRealValue()));
    timer->start(1000);
}

void AIpoint::getRealValue()
{
   realValue=ENG_MAX*(HoldRegistervalue[registerindex]-MINHOLDREGISTER)/MAXHOLDREGISTER+ENG_MIN;

}

void AIpoint::updateRealValue()
{
    getRealValue();

    /*GET ALARM caculate
        0-LL ENABLE
        1-L  ENABLE
        2-H  ENABLE
        3-HH ENABLE
        4-LL HAS ALARM
        5-L  HAS ALARM
        6-H  HAS ALARM
        7-HH HAS ALARM
    */
     if(isTrue(alarmMask,0) && realValue<LL) setTrue(alarmMask,4);
     if(isTrue(alarmMask,1) && realValue<LO) setTrue(alarmMask,5);
     if(isTrue(alarmMask,2) && realValue<HI) setTrue(alarmMask,6);
     if(isTrue(alarmMask,3) && realValue<HH) setTrue(alarmMask,7);
     /*display value,only in current graph can update*/
     if(currentDisplaypage==pageIndex)
     {
        label->setText(QString::number(realValue));
     }

}



