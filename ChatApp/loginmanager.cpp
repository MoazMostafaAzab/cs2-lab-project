#include "loginmanager.h"
#include "INetworkClient.h"

LoginManager::LoginManager(INetworkClient* network) : m_network(network), m_currentUser("") {}

bool LoginManager::login(const QString& username){
    if (username.isEmpty())
        return false;
    m_currentUser = username;
    m_network->connectToServer(m_currentUser);
    return true;
}

void LoginManager::logout(){
    m_currentUser = "";
    m_network->disconnect();
}

QString LoginManager::getCurrentUser() const{
    return m_currentUser;
}
