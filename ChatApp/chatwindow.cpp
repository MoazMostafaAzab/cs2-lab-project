#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::setUsername(const QString& username)
{
    m_username = username;
}

void ChatWindow::on_pushButton_chatwindowBack_clicked()
{
    this->close();
}

