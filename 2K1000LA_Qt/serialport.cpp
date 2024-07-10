#include "serialport.h"


SerialPort::SerialPort(int portId,QString port,long baud,QObject *parent): QObject(parent)
{
    my_thread = new QThread();
    m_portId = portId;
    show_fun_id();
    show_slots_id();
    m_port = new QSerialPort();

    init_port(port,baud);
    cun=0;
    this->moveToThread(my_thread);
    m_port->moveToThread(my_thread);
    my_thread->start();//
    qDebug()<<"in main thread";
}

SerialPort::~SerialPort()
{

}
void SerialPort::show_fun_id()
{
    qDebug()<<"fun_id is:"<<QThread::currentThreadId();
}
void SerialPort::show_slots_id()
{
    qDebug()<<"slots_id is:"<<QThread::currentThreadId();
    show_fun_id();
}
void SerialPort::init_port(QString port,long baud)
{
    m_port->setPortName(port);
    m_port->setBaudRate(baud);                  //波特率
    m_port->setDataBits(QSerialPort::Data8);    //数据位
    m_port->setParity(QSerialPort::NoParity);   //奇偶校验
    m_port->setStopBits(QSerialPort::OneStop);  //停止位
    m_port->setFlowControl(QSerialPort::NoFlowControl);//控制位

    if(!m_port->open(QIODevice::ReadWrite))//读写方式打开
    {
         qDebug()<<"open port "<<m_portId <<" failed!"<<endl;
    }
    else
    {
        qDebug()<<"Port "<<m_portId<<" have been opened.";
    }
    m_readTimer = new QTimer(this);
    m_readTimer->start(100); /*开启定时器，并且每100ms后询问一次串口。定时的时间一到，马上产生timeout（）信号，继续执行自定义槽函数readMyCom() */
    connect(m_readTimer,SIGNAL(timeout()),this,SLOT(read_data()));
//    connect(m_port,SIGNAL(readyRead()),this,SLOT(read_data()));//有数据就读
    connect(this,SIGNAL(thread_sig()),this,SLOT(show_slots_id()));
}

void SerialPort::closePort(int portId)
{
    if(portId==m_portId)
    {
        m_readTimer->stop();
     //   m_port->clear();
        m_port->close();
        m_port->deleteLater();

        my_thread->wait();
        my_thread->quit();
    }
    qDebug()<<m_portId<<" thread_id is:"<<QThread::currentThreadId();
    emit thread_sig();
}

void SerialPort::read_data()
{      
    QByteArray buff ;
    qint64 buffsize = m_port->size();//获取缓冲区数据的大小

    //qDebug()<<"portId: "<<m_portId<<" buffsize: "<<buffsize;
    cun++;
    if(buffsize>0)
    {
        buff = m_port->readAll();

        emit receive_data_all(m_portId, buff);//发送所有数据

        for (int i=0;i<buff.size();i++)
        {           
           emit receive_data(m_portId, buff.at(i));
        }
    }
}


/*
void SerialPort::read_data()
{
    QByteArray buff ;

    qint64 buffsize = m_port->size();//获取缓冲区数据的大小

    //qDebug()<<"portId: "<<m_portId<<" buffsize: "<<buffsize;
    cun++;
    if(buffsize>0)
    {
        buff = m_port->readAll();

        for (int i=0;i<buff.size();i++)
        {
           emit receive_data(m_portId, buff.at(i));
        }
    }
}
*/

 void SerialPort::write_data(int portId,QByteArray buff)
 {     
     if(portId==m_portId)
     {
         m_port->write(buff);
     }
//     qDebug()<<m_portId<<" write_id is:"<<QThread::currentThreadId();
//     emit thread_sig();
 }



