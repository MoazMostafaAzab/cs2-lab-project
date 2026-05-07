#ifndef CHATWINDOW_H
#define CHATWINDOW_H
#include <QDialog>
#include "chatmanager.h"

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
    void setChatManager(ChatManager* manager);

private:
    Ui::ChatWindow *ui;
    QString m_username;
    ChatManager* m_chatManager = nullptr;

private slots:
    void on_sendButton_clicked();
    void onMessageReceived(const QString &sender, const QString &text);
};

#endif // CHATWINDOW_H
