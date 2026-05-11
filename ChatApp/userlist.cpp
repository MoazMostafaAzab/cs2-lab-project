#include "userlist.h"
#include "ui_userlist.h"
#include "chatwindow.h"
#include "groupwindow.h"
#include "GroupManager.h"
#include "groupchatwindow.h"

UserList::UserList(INetworkClient* network, LoginManager* loginManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserList)
{
    ui->setupUi(this);
    this->m_loginManager=loginManager;
    this->m_network=network;
    m_groupManager = nullptr;
}

UserList::~UserList()
{
    delete ui;
}

/*void UserList::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current == nullptr) return;
    if(previous == nullptr) return;

    QString selectedUser = current->text();
    ChatWindow* chatWin = new ChatWindow(m_network, this);
    chatWin->setUsername(selectedUser);
    chatWin->show();
} */

ChatWindow* UserList::getOrOpenChat(const QString& username)
{

    if (m_chatWindows.contains(username)) {
        ChatWindow* existing = m_chatWindows[username];
        existing->show();
        existing->raise();
        existing->activateWindow();
        return existing;
    }


    ChatWindow* chatWin = new ChatWindow(m_network, nullptr);
    chatWin->setUsername(username);
    m_chatWindows[username] = chatWin;


    connect(chatWin, &QObject::destroyed, this, [this, username]() {
        m_chatWindows.remove(username);
    });

    chatWin->show();
    return chatWin;
}
GroupChatWindow* UserList::getOrCreateGroupChat(const QString& groupName)
{
    if (m_groupChatWindows.contains(groupName))
        return m_groupChatWindows[groupName];

    GroupChatWindow* win = new GroupChatWindow(nullptr, m_network, groupName);
    m_groupChatWindows[groupName] = win;

    connect(win, &QObject::destroyed, this, [this, groupName](){
        m_groupChatWindows.remove(groupName);
    });

    return win;
}
GroupChatWindow* UserList::getOrOpenGroupChat(const QString& groupName)
{
    GroupChatWindow* win = getOrCreateGroupChat(groupName);
    win->show();
    win->raise();
    win->activateWindow();
    return win;
}

void UserList::onAddedToGroup(const QString& groupName)
{
    QList<QListWidgetItem*> items =
        ui->listWidget_groups->findItems(groupName, Qt::MatchExactly);

    if(items.isEmpty())
        ui->listWidget_groups->addItem(groupName);
}

void UserList::onGroupMessageReceived(const QString& groupName,const QString& sender,const QString& text) {

        GroupChatWindow* win = getOrOpenGroupChat(groupName);
        win->appendMessage(sender, text);

}

void UserList::on_listWidget_groups_itemDoubleClicked(QListWidgetItem* item)
{
    if (item == nullptr) return;
    getOrOpenGroupChat(item->text());
}

void UserList::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
    if (item == nullptr) return;
    getOrOpenChat(item->text());
}

void UserList::on_pushButton_2_clicked()
{
    GroupWindow* GW = new GroupWindow(m_network, m_loginManager->getCurrentUser(), m_groupManager);
    connect(GW, &GroupWindow::openGroupChat,
            this, &UserList::getOrOpenGroupChat);

    GW->show();
}

void UserList::onRemovedFromGroup(const QString& groupName)
{

    if (m_groupChatWindows.contains(groupName)) {
        m_groupChatWindows[groupName]->close();
        m_groupChatWindows.remove(groupName);
    }


    QList<QListWidgetItem*> items =
        ui->listWidget_groups->findItems(groupName, Qt::MatchExactly);
    for (auto* item : items)
        delete item;
}

void UserList::onGroupCreated(const QString& groupName)
{
    QList<QListWidgetItem*> items =
        ui->listWidget_groups->findItems(groupName, Qt::MatchExactly);
    if (items.isEmpty())
        ui->listWidget_groups->addItem(groupName);
}

void UserList::initGroupManager()
{
    if (m_groupManager) delete m_groupManager;
    m_groupManager = new GroupManager(m_network, m_loginManager->getCurrentUser());
}

void UserList::on_pushButton_clicked()
{
    ui->listWidget->clear();
    emit logOut();
}

void UserList::update(QList <QString> usernames){
    ui->listWidget->clear();
    for(int i=0; i<usernames.size();i++)
    {
        ui->listWidget->addItem(usernames[i]);
    }
}
