#ifndef ONENET_H
#define ONENET_H

#include <QObject>
#include <wifi.h>
#include <base64.h>
#include <hmac_sha1.h>
#include <mqttkit.h>

#define PROID			"vQN1R5Q839"
#define ACCESS_KEY		"89cNQ5FnK3vDEZNb89//45W1sbMzh/lBmddN6YuJhZc="
#define DEVICE_NAME		"monitor"

#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"


class Onenet : public QObject
{
    Q_OBJECT
public:
    explicit Onenet(QObject *parent = nullptr);
    ~Onenet();
    unsigned char OTA_UrlEncode(char *sign);
    unsigned char OneNET_Authorization(char *ver, char *res, unsigned int et, char *access_key, char *dev_name,
                                                char *authorization_buf, unsigned short authorization_buf_len, bool flag);
    bool OneNet_DevLink(void);
    unsigned char OneNet_FillBuf(char *buf);
    void OneNet_SendData(void);
    void OneNET_Publish(const char *topic, const char *msg);
    void OneNET_Subscribe(void);
    void OneNet_RevPro(unsigned char *cmd);

    void Onenet_Connect();
    void OneNet_UploadData(double data1, double data2, double data3, double data4);

    double temp,press,conc,offset;

signals:



private:
    Wifi *my_wifi;
    base64 *my_base64;
    hmac_sha1 *my_hmac_sha1;
    MqttKit *my_MqttKit;
};

#endif // ONENET_H
