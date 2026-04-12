#ifndef USERLIST_H
#define USERLIST_H

#include "LoginManager.h"
#include "qlistwidget.h"
#include <QWidget>

namespace Ui {
class UserList;
}

class UserList : public QWidget
{
    Q_OBJECT

public:
    UserList(NetworkClient* network, LoginManager* loginManager,QWidget *parent = nullptr);
    ~UserList();
    void update(QList<QString> usernames);


signals:
    void logOut();

private slots:
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::UserList *ui;
    NetworkClient* m_network;
    LoginManager* m_loginManager;
};

#endif // USERLIST_H
