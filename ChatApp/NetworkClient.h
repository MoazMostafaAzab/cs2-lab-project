#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "qobject.h"
#include <QString>

class NetworkClient : public QObject {
    Q_OBJECT
public:


    // MAZEN — Login / Logout / User List
    NetworkClient();
    void connectToServer(const QString& username);
    void disconnect();
    void requestUserList();


    // KARIM — Send / Receive Messages

    void sendPrivateMessage(const QString& toUser, const QString& message);
    void sendBroadcastMessage(const QString& message);


    // ABDURRAHMAN — Group Management

    void createGroup(const QString& groupName);
    void addUserToGroup(const QString& groupName, const QString& username);
    void removeUserFromGroup(const QString& groupName, const QString& username);
    void sendGroupMessage(const QString& groupName, const QString& message);


    // MOAZ — Will mock all of the above


    ~NetworkClient() = default;
signals:
    void userListReceived(QList<QString> userList);
};



#endif // NETWORKCLIENT_H
