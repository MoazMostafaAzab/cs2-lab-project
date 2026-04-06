#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include <QString>
#include <QList>
#include <QMap>
#include "NetworkClient.h"

class GroupManager {
public:
    // Constructor — takes a NetworkClient pointer and the current user's username
    GroupManager(NetworkClient* network, const QString& currentuser);

    // Create a new group
    QString createGroup(const QString& groupName);

    // Add a user to an existing group
    QString addUserToGroup(const QString& groupName, const QString& username);

    // Remove a user from an existing group
    QString removeUserFromGroup(const QString& groupName, const QString& username);

    // Send a message to a group
    QString sendGroupMessage(const QString& groupName, const QString& message);

    // Get the list of members in a group
    QList<QString> getGroupMembers(const QString& groupName);

private:
    NetworkClient* networkClient;
    QString currentUser;
    QMap<QString, QList<QString>> groups; // groupName -> list of members
};


#endif // GROUPMANAGER_H
