#include "lora.h"
//e.g $GPGGA,1,2,3,4,5,

Lora::Lora(QObject *parent) : QObject(parent)
{
    Lora_Serial_Init();
    // 初始化QTimer并连接槽
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &Lora::send_data_to_cloud);
    dataTimer->start(2000); // 每两秒触发一次
}

Lora::~Lora()
{
    emit closePort_sig(COM0);
}

void Lora::Lora_Serial_Init()
{
    PortA = new SerialPort(COM0, "ttyS3", 9600);

    //接收从子线程传输数据的信号
    connect(PortA,SIGNAL(receive_data_all(int,QByteArray)),this,SLOT(decode_data(int,QByteArray)));//,Qt::QueuedConnection
    connect(this,SIGNAL(closePort_sig(int)),PortA,SLOT(closePort(int)));

    qDebug()<<"PortA open";
    PortA->write_data(COM0, "hello world PortA");
}

/* 解析数据包 */
int Lora::decode_data(int portID, QByteArray QByteArray_buf)
{
    buffer.append(QByteArray_buf); // 将接收到的数据添加到缓存中
    char *buf = QByteArray_buf.data();
    char tmp[10];
    char data1[100], data2[100], data3[100], data4[100], data5[100], data6[100];
    char data7[100], data8[100], data9[100];

    if (portID == COM0)
    {
        if (buf[0] != '$')
            return -1;
        else if (strncmp(buf+3, "GGA", 3) != 0 && strncmp(buf+3, "AGA", 3) != 0)
            return -1;
        else if (strstr(buf, ",,,"))
        {
            printf("Place to open area\n");
            return -1;
        }
        else if(strncmp(buf+3, "AGA", 3) == 0){
            printf("raw data: %s\n", buf);
            sscanf(buf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", tmp, data1, data2, data3, data4, data5, data6);
            //debug
            //qDebug()<<buf;


            if(strtod(data1, nullptr)>100 && strtod(data1, nullptr)<10000)
                ddata1 = strtod(data1, nullptr);
            if(strtod(data2, nullptr)>100 && strtod(data2, nullptr)<10000)
                ddata2 = strtod(data2, nullptr);
            if(strtod(data3, nullptr)>100 && strtod(data3, nullptr)<10000)
                ddata3 = strtod(data3, nullptr);
            if(strtod(data4, nullptr)>100 && strtod(data4, nullptr)<10000)
                ddata4 = strtod(data4, nullptr);
            if(strtod(data5, nullptr)>100 && strtod(data5, nullptr)<10000)
                ddata5 = strtod(data5, nullptr);
            if(strtod(data6, nullptr)>100 && strtod(data6, nullptr)<10000)
                ddata6 = strtod(data6, nullptr);

            ddata10 = qSqrt( qPow(ddata1 - ddata4, 2) + qPow(ddata2 - ddata5, 2) + qPow(ddata3 - ddata6, 2) ) / 5000.0;

            qDebug()<<"data1="<<ddata1;
            qDebug()<<"data2="<<ddata2;
            qDebug()<<"data3="<<ddata3;
            qDebug()<<"data4="<<ddata4;
            qDebug()<<"data5="<<ddata5;
            qDebug()<<"data6="<<ddata6;
            qDebug()<<"data10="<<ddata10;
            emit decode_complete_sig(COM0, ddata1, ddata2, ddata3, ddata4, ddata5, ddata6);
            return 0;
        }

        else if(strncmp(buf+3, "GGA", 3) == 0){
            printf("raw data1: %s\n", buf);
            sscanf(buf, "%[^,],%[^,],%[^,],%[^,]", tmp, data7, data8, data9);
            //debug
            //qDebug()<<buf;
            if(strtod(data7, nullptr)>100 && strtod(data7, nullptr)<20000)
                ddata7 = (strtod(data7, nullptr) - 1334.5)/137.39;
            if(strtod(data8, nullptr)-50000>100 && strtod(data8, nullptr)-50000<20000)
                ddata8 = (strtod(data8, nullptr)-50000)/10.0 - 16;
            if(strtod(data9, nullptr)>100 && strtod(data9, nullptr)<20000)
            {
                ddata9 = qAbs( strtod(data9, nullptr)*1.034-912 );
            }

            qDebug()<<"data7="<<ddata7;
            qDebug()<<"data8="<<ddata8;
            qDebug()<<"data9="<<ddata9;

            emit decode_complete_sig1(COM0, ddata7, ddata8, ddata9);
            return 0;
        }
    }
    else
        return -1;
}


// QTimer的槽函数，每两秒发送一次数据
void Lora::send_data_to_cloud()
{
    emit send_datatocloud_sig(ddata7, ddata8, ddata9, ddata10); // 发送数据
}
