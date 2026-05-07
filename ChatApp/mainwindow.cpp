#include "mainwindow.h"
#include "chatwindow.h"
#include "ui_mainwindow.h"
#include "NetworkClient.h"
#include "chatmanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_network = new NetworkClient(this);
    m_chatManager = new ChatManager(m_network, this);

    connect(m_network, &NetworkClient::dataReceived,
            m_chatManager, &ChatManager::handleIncomingData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginSuccess(const QString& username)
{
    ChatWindow* chat = new ChatWindow(this);
    chat->setUsername(username);
    chat->setChatManager(m_chatManager);
    chat->show();
    this->hide();
}
