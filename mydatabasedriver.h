#ifndef MYDATABASEDRIVER_H
#define MYDATABASEDRIVER_H

#include <QObject>
#include <QWidget>
#include "DataType.h"

struct MYSQL;
struct MYSQL_RES;



class MyDatabaseDriver : public QWidget
{
    Q_OBJECT
public:
    explicit MyDatabaseDriver(QWidget *parent = nullptr);

signals:
protected:
    MYSQL *mysql=0;
    MYSQL_RES *result=0;
private:
    void writeLog(QString str);
public:
    bool Init();
    void ClearAll();
    bool ConnectToDatabase(const char *host, const char* user, const char * pass, const char*db, unsigned short port = 3306, unsigned long flag = 0);
    bool Query(const char*sql,unsigned long sqllen=0);
    bool SetConnectTimeout(int sec);
    bool SetReconnect(bool isre = true);
    bool StoreResult();
    void FreeSult();

    /*Generated a sql syntax*/
    QString GetInsertSql(XDATA kv,QString table);
    bool InsertSql(XDATA kv,QString table);//insert non binary data
};

#endif // MYDATABASEDRIVER_H

