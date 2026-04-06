#include "GroupManager.h"
#include "NetworkClient.h"
using namespace std;


GroupManager::GroupManager(NetworkClient* network, const QString& currentuser){
    networkClient = network;
    currentUser = currentuser;
}

QString GroupManager::createGroup(const QString& groupName){
    if (groupName.isEmpty()){
        QString result = "Error! Group Name can't be empty!";
        return result;
    }else if (groups.contains(groupName)){
        QString result = "Error! Group already exists!";
        return result;
    }else{
        groups[groupName] = QList<QString>();
        groups[groupName].append(currentUser);
        if(networkClient != nullptr){
            networkClient->createGroup(groupName);
        }
        QString result = "Group created successfully!";
        return result;
    }
}

QString GroupManager::addUserToGroup(const QString& groupName, const QString& username){
    if (groupName.isEmpty()){
        QString result = "Error! Group Name can't be empty!";
        return result;
    }else if (username.isEmpty()){
        QString result = "Error! Username can't be empty!";
        return result;
    }else if(!groups.contains(groupName)){
        QString result = "Error! Group does not exist!";
        return result;
    }else if (groups[groupName].contains(username)){
        QString result = "Error! User already in group!";
        return result;
    }else{
        groups[groupName].append(username);
        if(networkClient != nullptr){
            networkClient->addUserToGroup(groupName, username);
        }

        QString result = "User added successfully!";
        return result;
    }
}

QString GroupManager::removeUserFromGroup(const QString& groupName, const QString& username){
    if (groupName.isEmpty()){
        QString result = "Error! Group Name can't be empty!";
        return result;
    }else if (username.isEmpty()){
        QString result = "Error! Username can't be empty!";
        return result;
    }else if(!groups.contains(groupName)){
        QString result = "Error! Group does not exist!";
        return result;
    }else if (username == currentUser){
        QString result = "Error! Admin can't be removed from the group";
        return result;
    }else if (!groups[groupName].contains(username)){
        QString result = "Error! User is not in group!";
        return result;
    }else{
        groups[groupName].removeAll(username);
        if(networkClient != nullptr){
            networkClient->removeUserFromGroup(groupName, username);
        }
        QString result = "User removed successfully!";
        return result;
    }
}

QString GroupManager::sendGroupMessage(const QString& groupName, const QString& message){
    if (groupName.isEmpty()){
        QString result = "Error! Group Name can't be empty!";
        return result;
    }else if (message.isEmpty()){
        QString result = "Error! Message can't be empty!";
        return result;
    }else if(!groups.contains(groupName)){
        QString result = "Error! Group does not exist!";
        return result;
    }else{
        if(networkClient != nullptr){
            networkClient->sendGroupMessage(groupName, message);
        }

        QString result = "Sent!";
        return result;
    }
}

QList<QString>GroupManager::getGroupMembers(const QString& groupName){
    if(groups.contains(groupName)){
        return groups[groupName];
    }else{
        return QList<QString>();
    }
}


