#ifndef INETWORKCLIENT_H
#define INETWORKCLIENT_H

#include <QByteArray>

class INetworkClient {
public:
    virtual ~INetworkClient() = default;

    virtual void connectToServer(const QString& username) = 0;
    virtual void disconnect() = 0;
    virtual void requestUserList() = 0;

    virtual void sendPrivateMessage(const QString& toUser, const QString& message) = 0;
    virtual void sendBroadcastMessage(const QString& message) = 0;

    virtual void createGroup(const QString& groupName) = 0;
    virtual void addUserToGroup(const QString& groupName, const QString& username) = 0;
    virtual void removeUserFromGroup(const QString& groupName, const QString& username) = 0;
    virtual void sendGroupMessage(const QString& groupName, const QString& message) = 0;
};

#endif
