#include "userlist.h"
#include "ui_userlist.h"
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

}


void UserList::on_pushButton_2_clicked()
{
    GroupWindow* GW = new GroupWindow;
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
