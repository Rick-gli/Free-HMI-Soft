#ifndef MODBUSDRIVER_H
#define MODBUSDRIVER_H

#include <QObject>
#include <QWidget>
#include <QModbusDataUnit>
#include "DataType.h"




class QModbusClient;
class QModbusReply;

class modbusDriver : public QWidget
{
    Q_OBJECT
public:
    explicit modbusDriver(QWidget *parent = nullptr);
    bool InitConnect(const char * ip,int port=502,int timeout=2000,int retry=5);
    void ClearAll();
    //bool ReadRegister(QModbusDataUnit::RegisterType,int start,unsigned int count,unsigned int serveradd);
    bool ReadRegister(ModbusCode type,int start,unsigned int count,unsigned int serveradd=1);

    bool ConnectState=false;
private:
    void writeLog(QString str);
private:
    QModbusReply *lastRequest;
    QModbusClient *modbusDevice;
private slots:
    void readReady();
};

#endif // MODBUSDRIVER_H
