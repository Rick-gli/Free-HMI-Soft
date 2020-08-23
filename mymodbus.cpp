#include "mymodbus.h"
#include <QModbusTcpClient>
#include <QDebug>
#include <QFile>
#include <QTime>
#include <QTimer>

 unsigned int HoldRegistervalue[HoldRegisterCount-HoldRegisterStart];
 unsigned int CoilsRegistervalue[CoilsRegisterCount-CoilsRegisterStart];
 unsigned int InputStateRegistervalue[InputStateRegisterCount-InputStateRegisterStart];
 unsigned int InputRegistervalue[InputRegistersCount-InputRegistersStart];

 MyModbus::MyModbus(QWidget *parent) : QWidget(parent)
{
//        QTimer *timer = new QTimer(this);
//        connect(timer, SIGNAL(timeout()), this, SLOT(ReconnectToSlave()));
//        timer->start(3000);
}

bool MyModbus::InitInterFace()
{
    QString sql="";
    CloseInterFace();

    modbusDevice = new QModbusTcpClient(this);

    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        writeLog("bool MyModbus::InitInterFace(): "+modbusDevice->errorString());
    });

    return true;

}

void MyModbus::CloseInterFace()
{
    if(modbusDevice)
    {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
        modbusDevice=nullptr;
    }
}

bool MyModbus::ConnectToSlave(const char * ip,int port,int timeout,int retry)
{
    if(!modbusDevice)
    {
        writeLog(tr("MyModbus::ConnectToSlave() failed, modbusDevices is not initilized"));
        return false;
    }


    modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter,ip);
    modbusDevice->setTimeout(timeout);
    modbusDevice->setNumberOfRetries(retry);

    if (!modbusDevice->connectDevice()) {
         writeLog(tr("Connect failed: ") + modbusDevice->errorString());
         return false;
    } else {
        writeLog(tr("MyModbus::ConnectToSlave() -> connect finished"));
    }
    connectedIp=ip;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ReconnectToSlave()));
    timer->start(3000);

    return true;
}

bool MyModbus::ReadFromSlave(ModbusCode type,int start,unsigned int count,unsigned int serveradd)
{
    if(!modbusDevice)
    {
        writeLog(tr("MyModbus::ConnectToSlave() failed, modbusDevices is not initilized"));
        return false;
    }

    if (auto *reply = modbusDevice->sendReadRequest(readRequest(type,start,count), serveradd)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MyModbus::onReadReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        writeLog(tr("MyModbus::ReadFromSlave() Read error: ")+modbusDevice->errorString());
    }


    return true;
}

bool MyModbus::WriteOneToSlave(ModbusCode type, int address, int *data, unsigned int serveradd)
{
    if(!modbusDevice)
    {
        writeLog(tr("MyModbus::ConnectToSlave() failed, modbusDevices is not initilized"));
        return false;
    }
     QModbusDataUnit writeUnit = writeRequest(type,address,1);
     //QModbusDataUnit::RegisterType table = writeUnit.registerType();
     writeUnit.setValue(0,(*data));
     if(auto *reply=modbusDevice->sendWriteRequest(writeUnit,serveradd))
     {
        if(reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    writeLog(tr("Write response error: %1 (Mobus exception: 0x%2)")
                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    writeLog(tr("Write response error: %1 (code: 0x%2)").
                             arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        }
        else
        {
            reply->deleteLater();
        }
     }
     else
     {
         writeLog("Write error: " + modbusDevice->errorString());
     }
     return true;

}

QModbusDataUnit MyModbus::readRequest(ModbusCode type,int start,unsigned int count) const
{
    QModbusDataUnit::RegisterType unit;
    switch(type)
    {
        case Coils:
            unit=QModbusDataUnit::Coils;
            break;
        case InputState:
            unit=QModbusDataUnit::DiscreteInputs;
            break;
        case HoldingRegisters:
            unit=QModbusDataUnit::HoldingRegisters;
            break;
        case InputRegisters:
            unit=QModbusDataUnit::InputRegisters;
            break;
    }
    return QModbusDataUnit(unit,start,count);
}

QModbusDataUnit MyModbus::writeRequest(ModbusCode type, int start, unsigned int count) const
{
    QModbusDataUnit::RegisterType unit;
    if(type == Coils)
    {
       unit= QModbusDataUnit::Coils;
    }
    if(type ==HoldingRegisters)
    {
       unit=QModbusDataUnit::HoldingRegisters;
    }
    return QModbusDataUnit(unit,start,count);
}

void MyModbus::onReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        CrrospondingData(unit);
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        writeLog(tr("Read response error: %1 (Mobus exception: 0x%2)").
                 arg(reply->errorString()).
                 arg(reply->rawResult().exceptionCode(), -1, 16));
    } else {
        writeLog(tr("Read response error: %1 (code: 0x%2)").
                 arg(reply->errorString()).
                 arg(reply->error(), -1, 16));
    }

    reply->deleteLater();

}

void MyModbus::writeLog(QString str)
{
    QFile log("../log/ModbusDriver.log");
    if (!log.open(QIODevice::Append | QIODevice::Text))
             return;
    QTextStream out(&log);
    QDateTime time(QDateTime::currentDateTime());
    out << "\n"<<str << ": " << time.toString("yyyy-MM-dd  HH:mm:ss");
    log.close();
}

void MyModbus::CrrospondingData(const QModbusDataUnit unit)
{
//    qDebug()<<unit.startAddress();
//    qDebug()<<unit.valueCount();
//    qDebug()<<unit.registerType();
    switch(unit.registerType())
    {
        case QModbusDataUnit::Coils:
            {
                for(int i=unit.startAddress(),total=int(unit.valueCount());i<total;i++)
                {
                   CoilsRegistervalue[i]=unit.value(i);
                }
                break;
            }

        case QModbusDataUnit::HoldingRegisters:
            {
                for(int i=unit.startAddress(),total=int(unit.valueCount());i<total;i++)
                {
                  HoldRegistervalue[i]=unit.value(i);
                }
                break;
            }
        case QModbusDataUnit::InputRegisters:
            {
                for(int i=unit.startAddress(),total=int(unit.valueCount());i<total;i++)
                {
                  InputRegistervalue[i]=unit.value(i);
                }
                break;
            }
        case QModbusDataUnit::DiscreteInputs:
            {
                for(int i=unit.startAddress(),total=int(unit.valueCount());i<total;i++)
                {
                  InputStateRegistervalue[i]=unit.value(i);
                }
                break;
            }
        default:
            break;
    }
}

void MyModbus::ReconnectToSlave()
{
    if(!modbusDevice) return;
    if(modbusDevice->state()!=QModbusDevice::ConnectedState)
    {
        ConnectToSlave(connectedIp);
        writeLog("Try to reconnect");
    }
}
