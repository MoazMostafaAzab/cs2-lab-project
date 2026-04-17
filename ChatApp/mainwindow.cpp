#include "mainwindow.h"
#include "chatwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(INetworkClient* network, QWidget *parent )
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_network(network)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginSuccess(const QString& username)
{
    ChatWindow* chat = new ChatWindow(m_network, this);
    chat->setUsername(username);
    chat->show();

    this->hide();
}
