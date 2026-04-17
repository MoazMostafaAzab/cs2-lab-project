#ifndef GROUPWINDOW_H
#define GROUPWINDOW_H
#include "GroupManager.h"
#include "NetworkClient.h"
#include <QWidget>
namespace Ui {
class GroupWindow;
}
class GroupWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GroupWindow(NetworkClient* networkClient, const QString& username, QWidget *parent = nullptr);
    ~GroupWindow();
private slots:
    void on_pushButton_CreateGroup_clicked();
    void on_pushButton_AddUser_clicked();
    void on_pushButton_RemoveUser_clicked();
    void on_pushButton_groupwindowBack_clicked();

private:
    Ui::GroupWindow *ui;
    GroupManager* groupManager;
};
#endif // GROUPWINDOW_H
