#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QString>
#include <NetworkClient.h>

class LoginManager
{
public:
    LoginManager(NetworkClient* network);
    bool login(const QString& username);
    void logout();
    QString getCurrentUser() const;

private:
    NetworkClient* m_network;
    QString m_currentUser;

};

#endif // LOGINMANAGER_H
