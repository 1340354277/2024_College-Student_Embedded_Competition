#ifndef LORA_H
#define LORA_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include "serialport.h"
#include "obsbuffer.h"
#include <QFile>
#include "qmath.h"

class Lora : public QObject
{
    Q_OBJECT
public:
    explicit Lora(QObject *parent = nullptr);
    ~Lora();
    void Lora_Serial_Init();
    double ddata1, ddata2, ddata3, ddata4, ddata5, ddata6;
    double ddata7, ddata8, ddata9, ddata10;

public slots:
    int decode_data(int portID, QByteArray QByteArray_buf);
    void send_data_to_cloud();
signals:
    void closePort_sig(int portId);
    void decode_complete_sig(int portId, double ddata1, double ddata2, double ddata3, double ddata4, double ddata5, double ddata6);
    void decode_complete_sig1(int portId, double ddata7, double ddata8, double ddata9);
    void send_datatocloud_sig(double temp, double press, double conc, double offset);

private:
    SerialPort *PortA;
    QByteArray buffer;
    QTimer *dataTimer; // 添加一个QTimer成员
};

#endif // LORA_H
