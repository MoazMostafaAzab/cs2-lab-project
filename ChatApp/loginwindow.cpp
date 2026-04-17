#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include "userlist.h"


LoginWindow::LoginWindow(LoginManager* manager, QWidget* parent)
    : QWidget(parent),
    ui(new Ui::LoginWindow),
    m_manager(manager)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_connectButton_clicked()
{
    QString username = ui->usernameInput->text();
    if (m_manager->login(username)){
        emit loginSuccessful(username);
    }
    else{
        QMessageBox::warning(this, "Error", "Username cannot be empty");
    }
}
