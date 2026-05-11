#ifndef USERLIST_H
#define USERLIST_H

#include "LoginManager.h"
#include "qlistwidget.h"
#include "INetworkClient.h"
#include "chatwindow.h"
#include "GroupManager.h"
#include <QWidget>
#include <QMap>

class GroupChatWindow;

namespace Ui {
class UserList;
}

class UserList : public QWidget
{
    Q_OBJECT

public:
    UserList(INetworkClient* network, LoginManager* loginManager,QWidget *parent = nullptr);
    ~UserList();
    void update(QList<QString> usernames);

    ChatWindow* getOrOpenChat(const QString& username);
    GroupChatWindow* getOrOpenGroupChat(const QString& groupName);
    GroupChatWindow* getOrCreateGroupChat(const QString& groupName);

    void onAddedToGroup(const QString& groupName);
    void onGroupMessageReceived(const QString& groupName, const QString& sender, const QString& text);
    void onRemovedFromGroup(const QString& groupName);
    void onGroupCreated(const QString& groupName);
    void initGroupManager();

signals:
    void logOut();

private slots:
    //void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
    void on_listWidget_groups_itemDoubleClicked(QListWidgetItem* item);
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::UserList *ui;
    INetworkClient* m_network;
    LoginManager* m_loginManager;
    QMap<QString, ChatWindow*> m_chatWindows;
    QMap<QString, GroupChatWindow*> m_groupChatWindows;
    GroupManager* m_groupManager;


};

#endif // USERLIST_H
