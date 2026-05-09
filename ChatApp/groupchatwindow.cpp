#include "groupchatwindow.h"
#include "ui_groupchatwindow.h"
#include "INetworkClient.h"
#include <QMessageBox>

GroupChatWindow::GroupChatWindow(QWidget *parent, INetworkClient* network, QString groupName)
    : QWidget(parent)
    , ui(new Ui::GroupChatWindow)
{
    ui->setupUi(this);
    this->network = network;
    this->groupName = groupName;
    ui->label_GroupName->setText(groupName);
}

GroupChatWindow::~GroupChatWindow()
{
    delete ui;
}



void GroupChatWindow::on_pushButton_Send_clicked()
{
    QString message;
    message = ui->lineEdit_TypMessage->text();

    if(message.isEmpty()){
        QMessageBox::warning(nullptr, "Error", "Message box can't be empty");
    }else {
        network->sendGroupMessage(groupName, message);
        ui->textBrowser_GroupMessages->append("You: " + message);
        ui->lineEdit_TypMessage->clear();
    }
}

void GroupChatWindow::appendMessage(const QString& sender, const QString& text)
{
    ui->textBrowser_GroupMessages->append(sender + ": " + text);
}

void GroupChatWindow::on_pushButton_Back_clicked()
{
    this->close();
}

