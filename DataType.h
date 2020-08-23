#ifndef DATATYPE_H
#define DATATYPE_H
#include<QLabel>
#include<QMap>
#include<string>
#include<QString>

/*==> modbus register define*/
#define CoilsRegisterStart 0
#define CoilsRegisterCount 1000

#define HoldRegisterStart 0
#define HoldRegisterCount 1000

#define InputStateRegisterStart 0
#define InputStateRegisterCount 1000

#define InputRegistersStart 0
#define InputRegistersCount 1000
/* modbus register define end*/

/*==> ai point define */
#define MAXAIPOINT 100
#define MAXHOLDREGISTER 32000
#define MINHOLDREGISTER 6400
/* ai point define end */


/* Miscellaneous */


#define isTrue(num,n)  ((num>>n)&1)
#define setTrue(num,n) (num|(1<<n))
/**/
enum PointType {
    DIGIT_OUT=1,
    DIGIT_INPUT,
    ANALOG_OUTPUT,
    ANAILOG_INPUT

};
enum ModbusCode{
    Coils=1,
    InputState,
    HoldingRegisters,
    InputRegisters

};


struct pageData{
    int pageIndex=0;
    int pointIndex=0;
    int plcValue=0;
    const char * name=0;
    QLabel *label;
    PointType TYPE;
    float ENG_MAX=100;
    float ENG_MIN=0;
    float realValue=0;
    float HI=0;
    float LO=0;
};
/*Global Variable*/

extern unsigned int HoldRegistervalue[HoldRegisterCount-HoldRegisterStart];
extern unsigned int CoilsRegistervalue[CoilsRegisterCount-CoilsRegisterStart];
extern unsigned int InputStateRegistervalue[InputStateRegisterCount-InputStateRegisterStart];
extern unsigned int InputRegistervalue[InputRegistersCount-InputRegistersStart];
/*Global Variable*/


/***MYSQL Data TYPE*****************************************************************************/
struct LGHData{
    const char* data=0;
    int size=0;
    int value=0;
};

//typedef std::map<std::string,LGHData> XDATA;
typedef QMap<QString,LGHData> XDATA;

/***MYSQL Data TYPE*****************************************************************************/
#endif // DATATYPE_H

