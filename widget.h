#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }

QT_END_NAMESPACE
struct pageData;
class MyDatabaseDriver;
class modbusDriver;
class MyModbus;
class AIpoint;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QList< pageData> page1;
    pageData *point;
    MyDatabaseDriver *mysql; //mysql interface
    MyModbus *modbus;
    AIpoint *ai;

private:
    Ui::Widget *ui;
private:
    void initPage(int pageindex);
    void updatePage(int pageindex);
    void bindingVariable();
public slots:
    void on_readbutton_clicked();
    void testmode();

};
#endif // WIDGET_H
