#ifndef GROUPWINDOW_H
#define GROUPWINDOW_H
#include "GroupManager.h"
#include "INetworkClient.h"
#include <QWidget>
namespace Ui {
class GroupWindow;
}
class GroupWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GroupWindow(INetworkClient* networkClient, const QString& username, GroupManager* groupManager, QWidget *parent = nullptr);
    ~GroupWindow();
private slots:
    void on_pushButton_CreateGroup_clicked();
    void on_pushButton_AddUser_clicked();
    void on_pushButton_RemoveUser_clicked();
    void on_pushButton_groupwindowBack_clicked();
    void on_pushButton_groupSendMessage_clicked();
signals:
    void openGroupChat(const QString& groupName);
private:
    Ui::GroupWindow *ui;
    GroupManager* groupManager;
    INetworkClient* m_network;
};
#endif // GROUPWINDOW_H
