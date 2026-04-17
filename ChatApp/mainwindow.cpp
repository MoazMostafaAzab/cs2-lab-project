#include "mainwindow.h"
#include "chatwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginSuccess(const QString& username)
{
    ChatWindow* chat = new ChatWindow(this);
    chat->setUsername(username);
    chat->show();

    this->hide();
}
