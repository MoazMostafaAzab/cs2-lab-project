#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QDialog>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

    void setUsername(const QString& username);

private:
    Ui::ChatWindow *ui;
    QString m_username;
};

#endif // CHATWINDOW_H
