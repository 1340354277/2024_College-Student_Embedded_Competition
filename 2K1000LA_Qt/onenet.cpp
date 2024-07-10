#include "onenet.h"

//extern unsigned char esp8266_buf[512];
//extern double temperature,humidity,Atmosphere,Vibrate;


Onenet::Onenet(QObject *parent) : QObject(parent)
{
    my_wifi = new Wifi();
    my_base64 = new base64();
    my_MqttKit = new MqttKit();
    my_hmac_sha1 = new hmac_sha1();

    Onenet_Connect();
    OneNet_UploadData(1, 2, 3, 4);
}

Onenet::~Onenet()
{

}

void Onenet::Onenet_Connect()
{
    qDebug()<<"Connect MQTTs Server..."<<"\r\n";
        my_wifi->sleepMsec(500);
    while(my_wifi->ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
        my_wifi->sleepMsec(500);
    qDebug()<<"NET OK"<<"\r\n";
    while(OneNet_DevLink())         //ONENET
        my_wifi->sleepMsec(500);
}

void Onenet::OneNet_UploadData(double data1, double data2, double data3, double data4)
{
    //onenet
    unsigned char *dataPtr = NULL;
    temp   = data1;
    press  = data2;
    conc   = data3;
    offset = data4;
    OneNet_SendData();
    my_wifi->ESP8266_Clear();

    dataPtr = my_wifi->ESP8266_GetIPD(0);
    printf("dataPtr = %s\r\n", dataPtr);
    if(dataPtr != NULL)
        OneNet_RevPro(dataPtr);

    my_wifi->sleepMsec(5);
}




unsigned char Onenet::OTA_UrlEncode(char *sign)
{

    char sign_t[40];
    unsigned char i = 0, j = 0;
    unsigned char sign_len = strlen(sign);

    if(sign == (void *)0 || sign_len < 28)
        return 1;

    for(; i < sign_len; i++)
    {
        sign_t[i] = sign[i];
        sign[i] = 0;
    }
    sign_t[i] = 0;

    for(i = 0, j = 0; i < sign_len; i++)
    {
        switch(sign_t[i])
        {
            case '+':
                strcat(sign + j, "%2B");j += 3;
            break;

            case ' ':
                strcat(sign + j, "%20");j += 3;
            break;

            case '/':
                strcat(sign + j, "%2F");j += 3;
            break;

            case '?':
                strcat(sign + j, "%3F");j += 3;
            break;

            case '%':
                strcat(sign + j, "%25");j += 3;
            break;

            case '#':
                strcat(sign + j, "%23");j += 3;
            break;

            case '&':
                strcat(sign + j, "%26");j += 3;
            break;

            case '=':
                strcat(sign + j, "%3D");j += 3;
            break;

            default:
                sign[j] = sign_t[i];j++;
            break;
        }
    }

    sign[j] = 0;

    return 0;
}

/*
************************************************************
*	函数名称：	OTA_Authorization
*
*	函数功能：	计算Authorization
*
*	入口参数：	ver：参数组版本号，日期格式，目前仅支持格式"2018-10-31"
*				res：产品id
*				et：过期时间，UTC秒值
*				access_key：访问密钥
*				dev_name：设备名
*				authorization_buf：缓存token的指针
*				authorization_buf_len：缓存区长度(字节)
*
*	返回参数：	0-成功	其他-失败
*
*	说明：		当前仅支持sha1
************************************************************
*/
#define METHOD		"sha1"
unsigned char Onenet::OneNET_Authorization(char *ver, char *res, unsigned int et, char *access_key, char *dev_name,
                                            char *authorization_buf, unsigned short authorization_buf_len, bool flag)
{
    size_t olen = 0;

    char sign_buf[64];								//保存签名的Base64编码结果 和 URL编码结果
    char hmac_sha1_buf[64];							//保存签名
    char access_key_base64[64];						//保存access_key的Base64编码结合
    char string_for_signature[72];					//保存string_for_signature,这个是加密的key

//----------------------------------------------------参数合法性--------------------------------------------------------------------
    if(ver == (void *)0 || res == (void *)0 || et < 1564562581 || access_key == (void *)0
        || authorization_buf == (void *)0 || authorization_buf_len < 120)
        return 1;

//----------------------------------------------------将access_key进行Base64解码----------------------------------------------------
    memset(access_key_base64, 0, sizeof(access_key_base64));
    my_base64->BASE64_Decode((unsigned char *)access_key_base64, sizeof(access_key_base64), &olen, (unsigned char *)access_key, strlen(access_key));

    printf("access_key_base64: %s\r\n", access_key_base64);


//----------------------------------------------------计算string_for_signature-----------------------------------------------------
    memset(string_for_signature, 0, sizeof(string_for_signature));
    if(flag)
        snprintf(string_for_signature, sizeof(string_for_signature), "%d\n%s\nproducts/%s\n%s", et, METHOD, res, ver);
    else
        //snprintf(string_for_signature, sizeof(string_for_signature), "%d\n%s\nproducts/%s/devices/%s\n%s", et, METHOD, res, dev_name, ver);
        snprintf(string_for_signature, sizeof(string_for_signature), "%d\n%s\nproducts/%s/devices/%s\n%s", et, METHOD, res, dev_name, ver);
    printf("string_for_signature: %s\r\n", string_for_signature);


    printf("hex_access_key_base64\r\n");
    for (size_t i = 0; i < sizeof(access_key_base64); ++i) {
        printf("%02X ", (unsigned char)access_key_base64[i]);
    }
    printf("\r\nhex_string_for_signature\r\n");
    for (size_t i = 0; i < sizeof(string_for_signature); ++i) {
        printf("%02X ", (unsigned char)string_for_signature[i]);
    }
    printf("\n");

//----------------------------------------------------加密-------------------------------------------------------------------------
    memset(hmac_sha1_buf, 0, sizeof(hmac_sha1_buf));

    my_hmac_sha1->hmac_sha1_start((unsigned char *)access_key_base64, strlen(access_key_base64),
                (unsigned char *)string_for_signature, strlen(string_for_signature),
                (unsigned char *)hmac_sha1_buf);

    printf("hmac_sha1_buf: %s\r\n", hmac_sha1_buf);
    for (size_t i = 0; i < sizeof(hmac_sha1_buf); ++i) {
        printf("%02X ", (unsigned char)hmac_sha1_buf[i]);
    }
    printf("\n");
//----------------------------------------------------将加密结果进行Base64编码------------------------------------------------------
    olen = 0;
    memset(sign_buf, 0, sizeof(sign_buf));
    my_base64->BASE64_Encode((unsigned char *)sign_buf, sizeof(sign_buf), &olen, (unsigned char *)hmac_sha1_buf, strlen(hmac_sha1_buf));
    printf("sign_buf = %s\r\n", sign_buf);
//----------------------------------------------------计算Token---------------------------------------------------
    OTA_UrlEncode(sign_buf);
    //UsartPrintf(USART_DEBUG, "sign_buf: %s\r\n", sign_buf);

//----------------------------------------------------Token--------------------------------------------------------------------
    if(flag)
        snprintf(authorization_buf, authorization_buf_len, "version=%s&res=products%%2F%s&et=%d&method=%s&sign=%s", ver, res, et, METHOD, sign_buf);
    else
    {
        //strcpy ( sign_buf, "FS09n74MEBNLvnZ5FrFgxE8%2F1ao%3D" );
        snprintf(authorization_buf, authorization_buf_len, "version=%s&res=products%%2F%s%%2Fdevices%%2F%s&et=%d&method=%s&sign=%s", ver, res, dev_name, et, METHOD, sign_buf);
        printf("Token: %s\r\n", authorization_buf);
    }


    return 0;
}





///==========================================================
//	函数名称	OneNet_DevLink
//
//	函数功能	与onenet创建连接
//
//	入口参数	无
//
//	返回参数	1-成功	0-失败
//
//	说明		与onenet平台建立连接
//=========================================================
bool Onenet::OneNet_DevLink(void)
{

    MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

    unsigned char *dataPtr;

    char authorization_buf[160];

    bool status = 1;

    printf("sign0\r\n");
    OneNET_Authorization("2018-10-31", PROID, 1956499200, ACCESS_KEY, DEVICE_NAME,
                                authorization_buf, sizeof(authorization_buf), 0);

    printf("OneNET_DevLink\r\n"
                            "NAME: %s,	PROID: %s,	KEY:%s\r\n"
                        , DEVICE_NAME, PROID, authorization_buf);
    printf("sign1\n");
    if(my_MqttKit->MQTT_PacketConnect(PROID, authorization_buf, DEVICE_NAME, 256, 1, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
    {
        printf("sign2\n");
        printf("authorization_buf1 = %s\r\n", authorization_buf);
        printf("mqttPacket._data = %s\r\n", mqttPacket._data);			//////////////////////////////////debug
        my_wifi->ESP8266_SendData(mqttPacket._data, mqttPacket._len);	//上传平台
        printf("sign3\n");
        dataPtr = my_wifi->ESP8266_GetIPD(250);							//等待平台响应
        printf("dataPtr = %s\r\n", dataPtr);							//////////////////////////////////debug
        printf("sign4\n");
        if(dataPtr != NULL)
        {
            printf("dataPtr != NULL\r\n");
            if(my_MqttKit->MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
            {
                switch(my_MqttKit->MQTT_UnPacketConnectAck(dataPtr))
                {
                    case 0:printf("Tips:	连接成功\r\n");status = 0;break;

                    case 1:printf("WARN:	连接失败：协议错误\r\n");break;
                    case 2:printf("WARN:	连接失败：非法的client id\r\n");break;
                    case 3:printf("WARN:	连接失败：服务器失败\r\n");break;
                    case 4:printf("WARN:	连接失败：用户名或密码错误\r\n");break;
                    case 5:printf("WARN:	连接失败：非法链接(如token非法)\r\n");break;

                    default:printf("ERR:	ERR: 连接失败：未知错误\r\n");break;
                }
            }

            //status = 0;
        }

        my_MqttKit->MQTT_DeleteBuffer(&mqttPacket);								//删包
        printf("sign5\n");
    }
    else
        printf("WARN:	MQTT_PacketConnect Failed\r\n");

    return status;

}





unsigned char Onenet::OneNet_FillBuf(char *buf)
{
    char text[48];

    memset(text, 0, sizeof(text));

//	strcpy(buf, "{\"id\":123,\"dp\":{");
    strcpy(buf, "{\"id\":123,\"dp\":{");

    memset(text, 0, sizeof(text));
    sprintf(text, "\"Temperature\":[{\"v\":%f}],", temp);
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "\"Press\":[{\"v\":%f}],",press);
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "\"Concentration\":[{\"v\":%f}],",conc);
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "\"Offset\":[{\"v\":%f}]",offset);
    strcat(buf, text);

    strcat(buf, "}}");

    return strlen(buf);

}
char buf[256];



//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：
//==========================================================
void Onenet::OneNet_SendData(void)
{

    MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//协议包

//	char buf[256];

    short body_len = 0, i = 0;
    printf("Tips:	OneNet_SendData-MQTT\r\n");
    memset(buf, 0, sizeof(buf));

    body_len = OneNet_FillBuf(buf);																	//获取当前需要发送的总长度

    if(body_len)
    {
        if(my_MqttKit->MQTT_PacketSaveData(PROID, DEVICE_NAME, body_len, NULL, &mqttPacket) == 0)				//封包
        {
            for(; i < body_len; i++)
                mqttPacket._data[mqttPacket._len++] = buf[i];

            my_wifi->ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//上传数据到平台
            printf("Send %d Bytes\r\n", mqttPacket._len);

            my_MqttKit->MQTT_DeleteBuffer(&mqttPacket);															//删包
        }
        else
            printf("WARN:	EDP_NewBuffer Failed\r\n");
    }
}



//==========================================================
//	函数名称：	OneNET_Publish
//
//	函数功能：	发布消息
//
//	入口参数：	topic：发布的主题
//				msg：消息内容
//
//	返回参数：	无
//
//	说明：
//==========================================================
void Onenet::OneNET_Publish(const char *topic, const char *msg)
{

    MQTT_PACKET_STRUCTURE mqtt_packet = {NULL, 0, 0, 0};						//协议包

    printf("Publish Topic: %s, Msg: %s\r\n", topic, msg);

    if(my_MqttKit->MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL0, 0, 1, &mqtt_packet) == 0)
    {
        my_wifi->ESP8266_SendData(mqtt_packet._data, mqtt_packet._len);					//向平台发送订阅请求

        my_MqttKit->MQTT_DeleteBuffer(&mqtt_packet);										//删包
    }
}

//==========================================================
//	函数名称：	OneNET_Subscribe
//
//	函数功能：	订阅
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：
//==========================================================
void Onenet::OneNET_Subscribe(void)
{

    MQTT_PACKET_STRUCTURE mqtt_packet = {NULL, 0, 0, 0};						//协议包

    char topic_buf[56];
    const char *topic = topic_buf;

    snprintf(topic_buf, sizeof(topic_buf), "$sys/%s/%s/cmd/#", PROID, DEVICE_NAME);

    printf( "Subscribe Topic: %s\r\n", topic_buf);

    if(my_MqttKit->MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL0, &topic, 1, &mqtt_packet) == 0)
    {
        my_wifi->ESP8266_SendData(mqtt_packet._data, mqtt_packet._len);					//向平台发送订阅请求

        my_MqttKit->MQTT_DeleteBuffer(&mqtt_packet);										//删包
    }
}


//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：
//==========================================================
void Onenet::OneNet_RevPro(unsigned char *cmd)
{

    char *req_payload = NULL;
    char *cmdid_topic = NULL;

    unsigned short topic_len = 0;
    unsigned short req_len = 0;

    unsigned char qos = 0;
    static unsigned short pkt_id = 0;

    unsigned char type = 0;

    short result = 0;

    char *dataPtr = NULL;
    char numBuf[10];
    int num = 0;

    type = my_MqttKit->MQTT_UnPacketRecv(cmd);
    switch(type)
    {
        case MQTT_PKT_PUBLISH:																//接受的publish消息

            result = my_MqttKit->MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
            if(result == 0)
            {
                char *data_ptr = NULL;

                printf("topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n", cmdid_topic, topic_len, req_payload, req_len);

                data_ptr = strstr(cmdid_topic, "request/");									//查找cmdid
                if(data_ptr)
                {
                    char topic_buf[80], cmdid[40];

                    data_ptr = strchr(data_ptr, '/');
                    data_ptr++;

                    memcpy(cmdid, data_ptr, 36);											//复制cmdid
                    cmdid[36] = 0;

                    snprintf(topic_buf, sizeof(topic_buf), "$sys/%s/%s/cmd/response/%s", PROID, DEVICE_NAME, cmdid);
                    OneNET_Publish(topic_buf, "ojbk");										//回复命令
                }
            }

        case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack

            if(my_MqttKit->MQTT_UnPacketPublishAck(cmd) == 0)
                printf("Tips:	MQTT Publish Send OK\r\n");

        break;

        case MQTT_PKT_SUBACK:																//发送Subscribe消息的Ack

            if(my_MqttKit->MQTT_UnPacketSubscribe(cmd) == 0)
                printf("Tips:	MQTT Subscribe OK\r\n");
            else
                printf("Tips:	MQTT Subscribe Err\r\n");

        break;

        default:
            result = -1;
        break;
    }

    my_wifi->ESP8266_Clear();									//清空缓存

    if(result == -1)
        return;

    dataPtr = strchr(req_payload, '}');					//搜索':'

    if(dataPtr != NULL)					//如果找到了
    {
        dataPtr++;

        while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
        {
            numBuf[num++] = *dataPtr++;
        }
        numBuf[num] = 0;

        num = atoi((const char *)numBuf);				//转为数值形式
        if(strstr((char *)req_payload, "redled"))		//搜索"redled"
        {
            if(num == 1)								//控制数据如果为1，代表开
            {
                printf("GOOOFFFF\r\n");
            }
            else if(num == 0)							//控制数据如果为0，代表关
            {
                printf("Tips:	ASDASDAD\r\n");
            }
        }
                                                        //下同
    }

    if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
    {
        MQTT_FreeBuffer(cmdid_topic);
        MQTT_FreeBuffer(req_payload);
    }
}


