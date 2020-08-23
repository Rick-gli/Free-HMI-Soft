#include "mydatabasedriver.h"
#include "mysql.h"
#include "SystemConstDefine.h"
#include<string>
#include <QFile>
#include <QTime>
#include<QDebug>
MyDatabaseDriver::MyDatabaseDriver(QWidget *parent) : QWidget(parent)
{

}

bool MyDatabaseDriver::Init()
{
    ClearAll();
    mysql=mysql_init(0);
    if(!mysql)
    {
        writeLog("MYSQL Initilaze Failed");
        return false;
    }
    else
    {
        writeLog("MyDatabaseDriver::Init() success ");
    }

    return true;
}

void MyDatabaseDriver::ClearAll()
{
    FreeSult();
    if(mysql)
    {
        mysql_close(mysql);
        mysql = NULL;
        writeLog("close the mysql interface");
    }
    else
        writeLog("mysql has already NULL");

}

bool MyDatabaseDriver::ConnectToDatabase(const char *host, const char *user, const char *pass, const char *db, unsigned short port, unsigned long flag)
{
    if (!mysql && !Init())
            {

                writeLog("Connect failed! mysql is not Init" );
                return false;
            }

            if (!mysql_real_connect(mysql, host, user, pass, db, port, 0, flag))
            {

               QString str = "Mysql Connect failed";
               str += mysql_error(mysql);
                writeLog(str);
                return false;
            }
            writeLog("mysql connect success" );
            return true;
}

bool MyDatabaseDriver::Query(const char *sql, unsigned long sqllen)
{
    QString str = "";
    if (!mysql)
            {

                writeLog("Query failed, mysql is not init");
                return false;
            }
            if (!sql)
            {

                 writeLog("Query failed, sql syntax is null");
                return false;
            }
            if (sqllen <= 0)
            {
                sqllen = (unsigned long)strlen(sql);
            }
            if (sqllen <= 0)
            {
                 writeLog("Query failed, sql syntax is empty");
                return false;
            }
            int re = mysql_real_query(mysql, sql, sqllen);
            if (re != 0)
            {
                str="";
                str="execute SQL command error: ";
                str+=mysql_error(mysql);
                 writeLog("Query failed, sql syntax is empty");
                return false;
            }
            str="";
            str="success executed SQL command: ";
            str+=sql;

            writeLog(str);
            return true;
}

bool MyDatabaseDriver::SetConnectTimeout(int sec)
{
   QString str="";
    if (!mysql)
       {

                writeLog("Query failed, mysql is not init");
                return false;
         }
    int re=mysql_options(mysql,MYSQL_OPT_CONNECT_TIMEOUT,&sec);
    if(re!=0)
    {
       str=" mysql_options failed";
       str+=mysql_error(mysql);
        writeLog(str);
        return false;
    }
    str="Set MYSQL connect timeout to:";
    str+=tr("%1s").arg(sec);
    writeLog(str);
    return true;
}

bool MyDatabaseDriver::SetReconnect(bool isre)
{
    QString str="";
     if (!mysql)
        {

                 writeLog("Query failed, mysql is not init");
                 return false;
          }
     int re=mysql_options(mysql,MYSQL_OPT_RECONNECT,&isre);
     if(re!=0)
     {
        str=" mysql_options failed";
        str+=mysql_error(mysql);
         writeLog(str);
         return false;
     }
     writeLog("Set MYSQL to auto reconnect");
     return true;
}

bool MyDatabaseDriver::StoreResult()
{
    QString str="";
    if (!mysql)
            {
        writeLog("Query failed, mysql is not init");
        return false;
            }
            FreeSult();
            result = mysql_store_result(mysql);
            if (!result)
            {
                str="StoreResult failed: ";
                str+=mysql_error(mysql);
                writeLog(str);
                return false;
            }
            return true;
}

void MyDatabaseDriver::FreeSult()
{
    if (result)
            {
                mysql_free_result(result);
                result = NULL;

    }
}

QString MyDatabaseDriver::GetInsertSql(XDATA kv, QString table)
{
    //insert into table (name,size) values("name1",1024)
    QString sql="";
    if(kv.empty() || table.isEmpty()) return "";
    sql = "insert into `";
    sql+=table;
    sql+="`";

    QString keys="";
    QString vals="";

    for(auto p=kv.constBegin();p!=kv.constEnd();p++)
    {
        keys+="`";
        keys+=p.key();
        keys+="`,";

        vals+="'";
        vals+=p.value().data;
        vals+="',";
    }
    keys.replace((keys.length()-1),1,"");
    vals.replace((vals.length()-1),1,"");

    sql+="(";
    sql+=keys;
    sql+=") values(";
    sql+=vals;
    sql+=")";

    return sql;
}

bool MyDatabaseDriver::InsertSql(XDATA kv, QString table)
{
   QString sql= GetInsertSql(kv,table);
   if (!mysql)
           {
       writeLog("Query failed, mysql is not init");
       return false;
           }
   if(sql.isEmpty()) return false;

   if(!Query(sql.toStdString().c_str()))
   {
       writeLog(sql+"Error");
       return false;
   }
    int num=mysql_affected_rows(mysql);
    if(num<=0)
        return false;
    return true;
}

void MyDatabaseDriver::writeLog(QString str)
{
    Q_UNUSED(str);
    QFile log("../log/mysql.log");
    if (!log.open(QIODevice::Append | QIODevice::Text))
             return;
    QTextStream out(&log);
    QDateTime time(QDateTime::currentDateTime());
   // QString t=time.toString()
    out << "\n"<<str << ": " << time.toString("yyyy-MM-dd  HH:mm:ss");
    //ut<<str<< ":" << time.toString("dd.MM.yyyy");
   log.close();
}










