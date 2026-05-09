#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "QMessageBox"

ChatWindow::ChatWindow(INetworkClient* network, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    m_chatManager = new ChatManager(network, this);
    connect(m_chatManager, &ChatManager::messageReceived,
            this, &ChatWindow::onMessageReceived);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::setUsername(const QString& username)
{
    m_username = username;
    setWindowTitle("Chat with " + username);
    ui->label_message->setText("Chat with " + username);
}

void ChatWindow::setChatManager(ChatManager* manager)
{
    m_chatManager = manager;

    connect(m_chatManager, &ChatManager::messageReceived,
            this, &ChatWindow::onMessageReceived);
}

void ChatWindow::on_sendButton_clicked()
{
    if (!m_chatManager) return;

    QString message = ui->messageInput->text().trimmed();
    if (message.isEmpty()){
        QMessageBox::warning(this, "Error", "Message cannot be empty.");
        return;
    }
    bool success = m_chatManager->sendMessage(m_username, message);

    if (success) {
        ui->chatDisplay->append("You: " + message);
        ui->messageInput->clear();
    }
}

void ChatWindow::onMessageReceived(const QString &sender, const QString &text)
{
    ui->chatDisplay->append(sender + ": " + text);
}

void ChatWindow::appendMessage(const QString& sender, const QString& text)
{
    ui->chatDisplay->append(sender + ": " + text);
}

void ChatWindow::on_pushButton_chatwindowBack_clicked()
{
    this->close();
}
