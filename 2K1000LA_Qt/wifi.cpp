#include "wifi.h"

Wifi::Wifi(QObject *parent) : QObject(parent)
{
    PortB = new SerialPort(COM1, "ttyS4", 115200);

    //接收从子线程传输数据的信号
    connect(PortB,SIGNAL(receive_data_all(int,QByteArray)),this,SLOT(ESP8266_Buf_trans(int,QByteArray)));//,Qt::QueuedConnection
    connect(this,SIGNAL(closePort_sig(int)),PortB,SLOT(closePort(int)));

    qDebug()<<"PortB open";
    PortB->write_data(COM1, "hello world PortB");
    Wifi_Serial_Init();
}

Wifi::~Wifi()
{
    emit closePort_sig(COM1);
}

void Wifi::Wifi_Serial_Init()
{
    /* WIFI初始化 */
    ESP8266_Clear();

    qDebug()<<"1. AT"<<"\r\n";
    while(ESP8266_SendCmd((char*)"AT\r\n", "OK"))
        QThread::msleep(500);

    qDebug()<<"2. CWMODE"<<"\r\n";
    while(ESP8266_SendCmd((char*)"AT+CWMODE=1\r\n", (char*)"OK"))
        QThread::msleep(500);

    qDebug()<<"3. AT+CWDHCP"<<"\r\n";
    while(ESP8266_SendCmd((char*)"AT+CWDHCP=1,1\r\n", (char*)"OK"))
        QThread::msleep(500);

    qDebug()<<"4. CWJAP"<<"\r\n";
    while(ESP8266_SendCmd((char*)ESP8266_WIFI_INFO, (char*)"GOT IP"))
        QThread::msleep(500);

    qDebug()<<"5. ESP8266 Init OK"<<"\r\n";
}

void Wifi::ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

void Wifi::ESP8266_Buf_trans(int portID, QByteArray QByteArray_buf)
{
    if (portID == COM1 && REV_FLAG == REV_WAIT)
    {
        //esp8266_buf = QByteArray_buf.data();        //将全部数据读取给buf
        // 获取指向数据的指针，并复制到 esp8266_buf 数组中
        const char* dataPtr = QByteArray_buf.constData();
        int dataSize = QByteArray_buf.size();
        memcpy(esp8266_buf, dataPtr, dataSize);
        esp8266_buf[dataSize] = '\0'; // 添加字符串结束符
        REV_FLAG = REV_OK;
    }
}


//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：
//==========================================================
bool Wifi::ESP8266_SendCmd(char *cmd, char *res)
{
    unsigned char timeOut = 200;
    PortB->write_data(COM1, cmd);

    while(timeOut--)
    {
        if(REV_FLAG == REV_OK)							//如果收到数据
        {
            REV_FLAG = REV_WAIT;
            printf("esp8266_buf = %s\r\n", esp8266_buf);
            if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
            {
                ESP8266_Clear();									//清空缓存
                return 0;
            }
        }
        sleepMsec(10);
    }
    return 1;
}


//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：
//==========================================================
/**/
void Wifi::ESP8266_SendData(unsigned char *data, unsigned short len)
{

    char cmdBuf[32];

    ESP8266_Clear();								//清空接收缓存
    sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
    if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到'>'时可以发送数据
    {
        //sleepMsec(10);
        printf("data = %s\r\n", data);
        // 将 unsigned char* 转换为 QByteArray
        QByteArray byteArray(reinterpret_cast<const char*>(data), len);

        PortB->write_data(COM1, byteArray);              //发送设备连接请求数据
        printf("byteArray = %s\r\n", byteArray);
    }
}

//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char * Wifi::ESP8266_GetIPD(unsigned short timeOut)
{

    char *ptrIPD = NULL;
    do
    {
        if(REV_FLAG == REV_OK)								//如果接收完成
        {
            REV_FLAG == REV_WAIT;
            ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//搜索"IPD"头
            printf("when search IPD esp8266_buf = %s\n", esp8266_buf);
            if(ptrIPD == NULL)											//如果没找到，可能是IPD头延迟
            {
                printf("\"IPD\" not found\r\n");
            }
            else
            {
                printf("ptrIPD1 = %s\r\n", ptrIPD);
                ptrIPD = strchr(ptrIPD, ':');	//找到':'

                printf("ptrIPD2 = %d\r\n", ptrIPD);
                if(ptrIPD != NULL)
                {
                    ptrIPD++;
                    printf("ptrIPD++ = %s\r\n", ptrIPD);
                    return (unsigned char *)(ptrIPD);
                }
                else
                {
                    printf("ptrIPD return NULL\r\n");
                    return NULL;
                }
            }
        }
        sleepMsec(5);													//
    } while(timeOut--);

    return NULL;														//
}



void  Wifi::sleepMsec(int msec)
{
    QTime curTimer = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < curTimer)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
