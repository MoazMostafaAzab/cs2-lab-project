#include "groupwindow.h"
#include "mainwindow.h"
#include "loginwindow.h"
#include "loginmanager.h"
#include "userlist.h"
#include "chatwindow.h"
#include "NetworkClient.h"
#include <QTimer>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetworkClient networkClient;
    LoginManager loginManager(&networkClient);
    LoginWindow loginWindow(&loginManager);
    UserList userListWindow(&networkClient, &loginManager);
    loginWindow.show();

    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful, [&](const QString& username){
        loginWindow.hide();
        userListWindow.show();
        QTimer::singleShot(500, [&](){
            networkClient.requestUserList();
        });

       // QList<QString> testUsers = {"User1", "User2", "User3"};
       // userListWindow.update(testUsers);
    });

    QObject::connect(&userListWindow, &UserList::logOut, [&](){
        loginWindow.show();
        userListWindow.close();
        loginManager.logout();
    });

    QObject::connect(&networkClient, &NetworkClient::userListReceived, [&](QList<QString> userList){
        userListWindow.update(userList);
    });


    QObject::connect(&networkClient, &NetworkClient::privateMessageReceived,
                     [&](const QString& sender, const QString& text){
        ChatWindow* chatWin = userListWindow.getOrOpenChat(sender);
        chatWin->appendMessage(sender, text);
                     });

    QObject::connect(&networkClient, &NetworkClient::addedToGroup,
                     [&](const QString& groupName){
                         userListWindow.onAddedToGroup(groupName);
                     });

    QObject::connect(&networkClient, &NetworkClient::groupMessageReceived,
                     [&](const QString& groupName, const QString& sender, const QString& text){
                         userListWindow.onGroupMessageReceived(groupName, sender, text);
                     });

    QObject::connect(&networkClient, &NetworkClient::removedFromGroup,
                     [&](const QString& groupName){
                         userListWindow.onRemovedFromGroup(groupName);
                     });
    QObject::connect(&networkClient, &NetworkClient::groupCreated,
                     [&](const QString& groupName){
                         userListWindow.onGroupCreated(groupName);
                     });

    return a.exec();
}
