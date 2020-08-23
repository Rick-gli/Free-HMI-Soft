#ifndef MYMODBUS_H
#define MYMODBUS_H

#include <QObject>
#include <QWidget>

#include <QModbusDataUnit>
#include "DataType.h"

class QModbusClient;
class QModbusReply;

class MyModbus : public QWidget
{
    Q_OBJECT
public:
    explicit MyModbus(QWidget *parent = nullptr);
    bool InitInterFace();
    void CloseInterFace();
    bool ConnectToSlave(const char * ip,int port=502,int timeout=2000,int retry=5);
    bool ReadFromSlave(ModbusCode type,int start,unsigned int count,unsigned int serveradd=1);
    bool WriteOneToSlave(ModbusCode type,int address,int * data=0,unsigned int serveradd=1);
private:
    QModbusDataUnit readRequest(ModbusCode type,int start,unsigned int count) const;
    QModbusDataUnit writeRequest(ModbusCode type,int start,unsigned int count) const;
    void onReadReady();
    void writeLog(QString str);
    void CrrospondingData(const QModbusDataUnit);
private:
    QModbusReply *lastRequest = nullptr;
    QModbusClient *modbusDevice = nullptr;
    const char * connectedIp=0;
public slots:
    void ReconnectToSlave();

};

#endif // MYMODBUS_H
