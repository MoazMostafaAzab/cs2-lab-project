#include "NetworkClient.h"

NetworkClient::NetworkClient(){

}

void NetworkClient::connectToServer(const QString &username)
{

}

void NetworkClient::disconnect()
{

}

void NetworkClient::requestUserList()
{
    emit userListReceived({"Khattab","Jeff"});
}

void NetworkClient::sendPrivateMessage(const QString &toUser, const QString &message)
{

}

void NetworkClient::sendBroadcastMessage(const QString &message)
{

}

void NetworkClient::createGroup(const QString &groupName)
{

}

void NetworkClient::addUserToGroup(const QString &groupName, const QString &username)
{

}

void NetworkClient::removeUserFromGroup(const QString &groupName, const QString &username)
{

}

void NetworkClient::sendGroupMessage(const QString &groupName, const QString &message)
{

}
