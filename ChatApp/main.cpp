#include "groupwindow.h"
#include "mainwindow.h"
#include "loginwindow.h"
#include "loginmanager.h"
#include "userlist.h"
#include "NetworkClient.h"
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
        networkClient.requestUserList();
        QList<QString> testUsers = {"User1", "User2", "User3"};
        userListWindow.update(testUsers);
    });

    QObject::connect(&userListWindow, &UserList::logOut, [&](){
        loginWindow.show();
        userListWindow.close();
        loginManager.logout();
    });

    QObject::connect(&networkClient, &NetworkClient::userListReceived, [&](QList<QString> userList){
        userListWindow.update(userList);
    });



    return a.exec();
}
