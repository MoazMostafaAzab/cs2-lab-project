#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include <QString>
#include <QList>
#include <QMap>
#include "INetworkClient.h"

class GroupManager {
public:

    GroupManager(INetworkClient* network, const QString& currentuser);


    QString createGroup(const QString& groupName);


    QString addUserToGroup(const QString& groupName, const QString& username);


    QString removeUserFromGroup(const QString& groupName, const QString& username);


    QString sendGroupMessage(const QString& groupName, const QString& message);


    QList<QString> getGroupMembers(const QString& groupName);

private:
    INetworkClient* networkClient;
    QString currentUser;
    QMap<QString, QList<QString>> groups; // groupName -> list of members
};


#endif // GROUPMANAGER_H
