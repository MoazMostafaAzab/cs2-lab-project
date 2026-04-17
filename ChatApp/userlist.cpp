#include "userlist.h"
#include "ui_userlist.h"
#include "chatwindow.h"
#include "groupwindow.h"

UserList::UserList(NetworkClient* network, LoginManager* loginManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserList)
{
    ui->setupUi(this);
    this->m_loginManager=loginManager;
    this->m_network=network;
}

UserList::~UserList()
{
    delete ui;
}

void UserList::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current == nullptr) return;
    if(previous == nullptr) return;

    QString selectedUser = current->text();
    ChatWindow* chatWin = new ChatWindow(this);
    chatWin->setUsername(selectedUser);
    chatWin->show();
}


void UserList::on_pushButton_2_clicked()
{
    GroupWindow* GW = new GroupWindow(m_network, m_loginManager->getCurrentUser());
    GW->show();
}


void UserList::on_pushButton_clicked()
{
    ui->listWidget->clear();
    emit logOut();
}

void UserList::update(QList <QString> usernames){
    for(int i=0; i<usernames.size();i++)
    {
        ui->listWidget->addItem(usernames[i]);
    }
}
