#ifndef HMAC_SHA1_H
#define HMAC_SHA1_H

#include <QObject>

#define MAX_MESSAGE_LENGTH		4096


class hmac_sha1 : public QObject
{
    Q_OBJECT
public:
    explicit hmac_sha1(QObject *parent = nullptr);
    unsigned long int ft(
        int t,
        unsigned long int x,
        unsigned long int y,
        unsigned long int z
    );

    unsigned long int k(int t);
    unsigned long int rotr(int bits, unsigned long int a);
    unsigned long int rotl(int bits, unsigned long int a);
    void sha1(
        unsigned char *message,
        int message_length,
        unsigned char *digest
    );

    void hmac_sha1_start(
        unsigned char *key,
        int key_length,
        unsigned char *data,
        int data_length,
        unsigned char *digest
    );


signals:

};

#endif // HMAC_SHA1_H
