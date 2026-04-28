#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QString>
#include <INetworkClient.h>

class LoginManager
{
public:
    LoginManager(INetworkClient* network);
    bool login(const QString& username);
    void logout();
    QString getCurrentUser() const;

private:
    INetworkClient* m_network;
    QString m_currentUser;

};

#endif // LOGINMANAGER_H
