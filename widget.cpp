#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QDebug>

#include "DataType.h"
#include "mydatabasedriver.h"
#include "modbusdriver.h"
#include"mythread.h"

#include "mymodbus.h"
#include "aipoint.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    point = new pageData[1000];
    mysql = new MyDatabaseDriver;
    modbus=new MyModbus(this);
    ai = new AIpoint[MAXAIPOINT];


    bindingVariable();
    initPage(0);
    /* 1S updata graphes */
//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(testmode()));
//    timer->start(1000);

   // mysql->writeLog("sss");
    mysql->Init();//test
    mysql->SetConnectTimeout(5);
    mysql->SetReconnect();
    mysql->ConnectToDatabase("127.0.0.1","root","Deltav123","test");//test

//    /*  3、执行语句
//     *  CREATE DATABASE IF NOT EXISTS `t_r`(`id` INT AUTO_INCREMENT,`name` VARCHAR(1024),`data` BLOB,`size` INT, PRIMARY KEY(id))
//     */

    QString sql = "CREATE TABLE IF NOT EXISTS `t_r` \
            (`id` INT AUTO_INCREMENT,\
            `name` VARCHAR(1024),\
            `data` BLOB,\
            `size` INT,\
            PRIMARY KEY(id))";

    mysql->Query(sql.toStdString().c_str());

    sql = "TRUNCATE t_r";

    mysql->Query(sql.toStdString().c_str());

   XDATA dd;
   LGHData lg1;
   LGHData lg2;
   lg1.data="t01";
   lg2.data="123";
   dd["name"] = lg1;
   dd["size"] = lg2;
    qDebug()<<"==look at=========="<<mysql->GetInsertSql(dd,"t_r");
    //mysql->Query((mysql->GetInsertSql(dd,"t_r")).toStdString().c_str());
    mysql->InsertSql(dd,"t_r");
    mysql->ClearAll();//test




//    modbus->InitInterFace();
//    modbus->ConnectToSlave("127.0.0.1");
//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(testmode()));
//    timer->start(1000);
//    ai[0].bingParameter(ui->label_11,1,"ppp");
//    ai[1].bingParameter(ui->label_12,2,"ppp");
//    ai[2].bingParameter(ui->label_13,3,"ppp");
//    ai[3].bingParameter(ui->label_14,4,"ppp");
//    ai[4].bingParameter(ui->label_15,5,"ppp");
//    ai[5].bingParameter(ui->label_16,6,"ppp");
//    ai[6].bingParameter(ui->label_17,7,"ppp");
//    ai[7].bingParameter(ui->label_18,8,"ppp");
//    ai[8].bingParameter(ui->label_19,9,"ppp");
//    ai[9].bingParameter(ui->label_20,10,"ppp");

}

Widget::~Widget()
{
    delete ui;
    delete [] point;
    delete mysql;
    delete modbus;
    delete [] ai;
}

void Widget::initPage(int pageindex)
{
    Q_UNUSED(pageindex);
//    pageData temp1;
//    temp1.label=(ui->label_11);
//    page1.append(temp1);
// ((QLabel*) ui->verticalLayout_2->itemAt(0)->widget())->setText(tr("aaaa"));
//  qDebug()<<ui->verticalLayout_2->layout()->count();
           // ui->label_11->sett
//ui->verticalLayout_2->
    ui->name0->setText(point[0].name);

}

void Widget::updatePage(int pageindex)
{
    switch (pageindex)
    {
        case 1:
            for(int i=0;i<page1.size();i++)
            {

            }
            break;
    case 2:
            break;
    default:
            break;

    }
}

void Widget::bindingVariable()
{
    point[0].name="pit-101";
    point[0].label=ui->label_11;




}

void Widget::on_readbutton_clicked()
{
    int var = 111;
    //modbus->WriteOneToSlave(ModbusCode::HoldingRegisters,8,&var);
    modbus->WriteOneToSlave(ModbusCode::Coils,8,&var);
}

void Widget::testmode()
{
    modbus->ReadFromSlave(ModbusCode::HoldingRegisters,0,100);
  //  ((QLabel*) ui->verticalLayout_2->itemAt(0))->setText(QString::number(HoldRegistervalue[49]));
 //   ui->label_11->setText(QString::number(2.3));
//    ui->label_12->setText(QString::number(HoldRegistervalue[1]));
//    ui->label_13->setText(QString::number(HoldRegistervalue[2]));
//    ui->label_14->setText(QString::number(HoldRegistervalue[3]));
//    ui->label_15->setText(QString::number(HoldRegistervalue[4]));
//    ui->label_16->setText(QString::number(HoldRegistervalue[5]));
//    ui->label_17->setText(QString::number(HoldRegistervalue[6]));
//    ui->label_18->setText(QString::number(HoldRegistervalue[7]));
//    ui->label_19->setText(QString::number(HoldRegistervalue[8]));
//    ui->label_20->setText(QString::number(HoldRegistervalue[9]));
}

