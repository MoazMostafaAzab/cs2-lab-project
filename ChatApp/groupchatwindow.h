#ifndef GROUPCHATWINDOW_H
#define GROUPCHATWINDOW_H

#include <QWidget>

namespace Ui {
class GroupChatWindow;
}

class GroupChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GroupChatWindow(QWidget *parent = nullptr);
    ~GroupChatWindow();

private:
    Ui::GroupChatWindow *ui;
};

#endif // GROUPCHATWINDOW_H
