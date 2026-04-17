#ifndef INETWORKCLIENT_H
#define INETWORKCLIENT_H

#include <QByteArray>

class INetworkClient {
public:
    virtual ~INetworkClient() = default;
    virtual void sendData(const QByteArray& data) = 0;
};

#endif
