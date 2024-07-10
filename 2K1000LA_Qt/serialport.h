#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>

#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QThread>
#include <QByteArray>
#include <QTimer>



class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(int portId,QString port,long baud,QObject *parent = nullptr);
    ~SerialPort();
    void show_fun_id();
    void init_port(QString port,long baud);//初始化串口

public slots:
    void write_data(int portId,QByteArray buff);//写串口数据 /*QByteArray*/
    void closePort(int portId);

private slots:
    void show_slots_id();
    void read_data();//读取串口数据

signals:
    void thread_sig();

    void receive_data(int portId,char buff);//向主线程发送接收到的串口数据
    void receive_data_all(int portId, QByteArray buff);//向主线程发送接收到的所有串口数据

private:
    QThread *my_thread;
    QSerialPort *m_port;//实例化一个指向串口的指针，用于访问串口
    int m_portId;
    QTimer *m_readTimer;

   int cun;

};

#endif // SERIALPORT_H
