#ifndef GROUPCHATWINDOW_H
#define GROUPCHATWINDOW_H

#include <QWidget>
#include "INetworkClient.h"

namespace Ui {
class GroupChatWindow;
}

class GroupChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GroupChatWindow(QWidget *parent = nullptr, INetworkClient* network = nullptr, QString groupName = "");
    ~GroupChatWindow();

private slots:
    void on_pushButton_Send_clicked();
    void on_pushButton_Back_clicked();

private:
    Ui::GroupChatWindow *ui;
    INetworkClient* network;
    QString groupName;
};

#endif // GROUPCHATWINDOW_H
