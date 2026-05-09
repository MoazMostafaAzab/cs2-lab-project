#include "groupwindow.h"
#include "ui_groupwindow.h"
#include "groupchatwindow.h"
#include <QMessageBox>

GroupWindow::GroupWindow(INetworkClient* networkClient, const QString& username, GroupManager* groupManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GroupWindow)
{
    ui->setupUi(this);
    this->groupManager = groupManager;
    m_network = networkClient;
}

GroupWindow::~GroupWindow()
{
    delete ui;
}

void GroupWindow::on_pushButton_CreateGroup_clicked()
{

    QString groupName = ui->lineEdit_GroupName->text();
    QString result = groupManager->createGroup(groupName);

    if(result == "Group created successfully!"){
        QMessageBox::information(this, "Success", result);
    } else {
        QMessageBox::warning(this, "Error", result);
    }

    ui->listWidget_GroupMembers->clear();
    QList<QString> members = groupManager->getGroupMembers(groupName);
    for(int i = 0; i < members.size(); i++){
        ui->listWidget_GroupMembers->addItem(members[i]);
    }

  //  ui->lineEdit_GroupName->clear();
}


void GroupWindow::on_pushButton_AddUser_clicked()
{
    QString User = ui->lineEdit_Username->text();
    QString groupName = ui->lineEdit_GroupName->text();
    QString result = groupManager->addUserToGroup(groupName, User);

    if(result == "User added successfully!"){
        QMessageBox::information(this, "Success", result);
    } else {
        QMessageBox::warning(this, "Error", result);
    }

    ui->listWidget_GroupMembers->clear();
    QList<QString> members = groupManager->getGroupMembers(groupName);
    for(int i = 0; i < members.size(); i++){
        ui->listWidget_GroupMembers->addItem(members[i]);
    }

   //  ui->lineEdit_GroupName->clear();
}


void GroupWindow::on_pushButton_RemoveUser_clicked()
{
    QString User = ui->lineEdit_Username->text();
    QString groupName = ui->lineEdit_GroupName->text();
    QString result = groupManager->removeUserFromGroup(groupName, User);

    if(result == "User removed successfully!"){
        QMessageBox::information(this, "Success", result);
    } else {
        QMessageBox::warning(this, "Error", result);
    }

    ui->listWidget_GroupMembers->clear();
    QList<QString> members = groupManager->getGroupMembers(groupName);
    for(int i = 0; i < members.size(); i++){
        ui->listWidget_GroupMembers->addItem(members[i]);
    }

    // ui->lineEdit_GroupName->clear();
}


void GroupWindow::on_pushButton_groupSendMessage_clicked()
{
    QString groupName = ui->lineEdit_GroupName->text();
    if(groupName.isEmpty()){
        QMessageBox::warning(this, "Error", "Please enter a group name first");
        return;
    }
    emit openGroupChat(groupName);
}

void GroupWindow::on_pushButton_groupwindowBack_clicked()
{
    this->close();
}

