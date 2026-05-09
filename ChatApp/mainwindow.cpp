#include "mainwindow.h"
#include "chatwindow.h"
#include "ui_mainwindow.h"
#include "NetworkClient.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_network = new NetworkClient(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginSuccess(const QString& username)
{
    ChatWindow* chatWin = new ChatWindow(m_network, this);

    chatWin->setUsername(username);
    chatWin->show();
    this->hide();
}
