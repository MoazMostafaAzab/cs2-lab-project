#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QMessageBox>

ChatWindow::ChatWindow(INetworkClient* network, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::ChatWindow),
    m_network(network)
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


void ChatWindow::on_sendButton_clicked()
{
    QString message = ui->messageInput->text().trimmed();

    if (message.isEmpty()) {
        QMessageBox::warning(this, "Error", "Message cannot be empty!");
        return;
    }

    if (m_network) {
        m_network->sendPrivateMessage(m_username, message);
    }

    QMessageBox::information(this, "Success", "Message sent successfully!");

    ui->messageInput->clear();
}

