#ifndef WIFI_H
#define WIFI_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include "serialport.h"
#include "obsbuffer.h"
#include <QFile>
#include <QTime>
#include <QCoreApplication>


#define     REV_OK      0
#define     REV_WAIT    1
#define     ESP8266_WIFI_INFO		"AT+CWJAP=\"ppad\",\"1340354277\"\r\n"

class Wifi : public QObject
{
    Q_OBJECT
public:
    explicit Wifi(QObject *parent = nullptr);
    ~Wifi();
    void Wifi_Serial_Init();
    void ESP8266_Clear(void);
    bool ESP8266_SendCmd(char *cmd, char *res);
    void ESP8266_SendData(unsigned char *data, unsigned short len);
    unsigned char *ESP8266_GetIPD(unsigned short timeOut);
    void sleepMsec(int msec);

    unsigned char esp8266_buf[512];//创建char指针并分配空间
    volatile bool REV_FLAG = REV_WAIT;

    unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;

public slots:
    void ESP8266_Buf_trans(int portID, QByteArray QByteArray_buf);

signals:
    void closePort_sig(int portId);

private:
    SerialPort *PortB;

};

#endif // WIFI_H
