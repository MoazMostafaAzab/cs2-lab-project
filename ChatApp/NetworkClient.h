#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QString>

class NetworkClient {
public:


    // MAZEN — Login / Logout / User List

    virtual void connectToServer(const QString& username) = 0;
    virtual void disconnect() = 0;
    virtual void requestUserList() = 0;


    // KARIM — Send / Receive Messages

    virtual void sendPrivateMessage(const QString& toUser, const QString& message) = 0;
    virtual void sendBroadcastMessage(const QString& message) = 0;


    // ABDURRAHMAN — Group Management

    virtual void createGroup(const QString& groupName) = 0;
    virtual void addUserToGroup(const QString& groupName, const QString& username) = 0;
    virtual void removeUserFromGroup(const QString& groupName, const QString& username) = 0;
    virtual void sendGroupMessage(const QString& groupName, const QString& message) = 0;


    // MOAZ — Will mock all of the above


    virtual ~NetworkClient() = default;
};

#endif // NETWORKCLIENT_H
