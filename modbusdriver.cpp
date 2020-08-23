#include "modbusdriver.h"
#include <QModbusTcpClient>
#include <QFile>
#include <QTime>
#include<QDebug>

modbusDriver::modbusDriver(QWidget *parent) : QWidget(parent), modbusDevice(nullptr)
{

}

bool modbusDriver::InitConnect(const char * ip,int port,int timeout,int retry)
{
    QString str="";
    ClearAll();
    modbusDevice=new QModbusTcpClient(this);

    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        writeLog(modbusDevice->errorString());
    });

    if(!modbusDevice)
    {
        writeLog("modbusDevice if not initialed");
        ConnectState=false;
        return false;
    }
    /*****start connect***********************/


        modbusDevice->setTimeout(timeout);
        modbusDevice->setNumberOfRetries(retry);
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter,port);
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter,ip);
        if(!modbusDevice->connectDevice())
        {
           str="modbus device connect failed: ";
           str+=modbusDevice->errorString();
           writeLog(str);
           return false;
        }
        else
        {
            writeLog("modbus device connect success");

            return true;
        }


}

void modbusDriver::ClearAll()
{
    if(modbusDevice)
    {
    modbusDevice->disconnectDevice();
    delete modbusDevice;
    }




}

bool modbusDriver::ReadRegister(ModbusCode type , int start, unsigned int count,unsigned int serveradd)
{
   QString str="";
    if(!modbusDevice)
   {
        writeLog("modbusDevice has not initilazed");
        return false;
   }
    if(count<=0)
    {
        writeLog("Must read greater than 1 register");
        return false;
    }
    QModbusDataUnit::RegisterType ttp;
    switch (type)
        {
            case Coils:
            {
                ttp=QModbusDataUnit::Coils;
                break;
            }

            case InputState:
            {

               ttp=QModbusDataUnit::InputRegisters;
                break;
            }

            case HoldingRegisters:
            {
                ttp=QModbusDataUnit::HoldingRegisters;
                break;
            }

            case InputRegisters:
            {
                ttp=QModbusDataUnit::DiscreteInputs;
                break;
            }
    }

    QModbusDataUnit readunit(ttp,start,count);
    auto *reply = modbusDevice->sendReadRequest(readunit,serveradd);
    if(reply)
    {
        if(reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, &modbusDriver::readReady);
        }
        else
        {
            delete reply;
        }
    }
    else
    {
      str="";
      str+="modbus read error: ";
      str+=modbusDevice->errorString();
      return false;
    }
    return true;
}
void modbusDriver::readReady()
{
    auto reply=qobject_cast<QModbusReply *>(sender());
    if(!reply)
    {
        writeLog("read request failed because sender is empty");
        return ;
    }
    if(reply->error() == QModbusDevice::NoError)
    {
       const QModbusDataUnit unit = reply->result();
       qDebug()<< "value 0=" << unit.value(0);
       qDebug()<< "value 1=" << unit.value(1);
    }
    else if(reply->error() == QModbusDevice::ProtocolError)
    {
      writeLog(tr("Read response error: %1 (Mobus exception: 0x%2)").
               arg(reply->errorString()).
               arg(reply->rawResult().exceptionCode(), -1, 16))  ;
    }
    else
    {
        writeLog(tr("Read response error: %1 (code: 0x%2)").
                 arg(reply->errorString()).
                 arg(reply->error(), -1, 16));
    }
    reply->deleteLater();
}
void modbusDriver::writeLog(QString str)
{
    QFile log("../log/ModbusDriver.log");
    if (!log.open(QIODevice::Append | QIODevice::Text))
             return;
    QTextStream out(&log);
    QDateTime time(QDateTime::currentDateTime());
    out << "\n"<<str << ": " << time.toString("yyyy-MM-dd  HH:mm:ss");
    log.close();
}


