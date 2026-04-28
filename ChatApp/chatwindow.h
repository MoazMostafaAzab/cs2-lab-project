#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QDialog>
#include "INetworkClient.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChatWindow(INetworkClient* network, QWidget *parent = nullptr);
    ~ChatWindow();

    void setUsername(const QString& username);

private slots:
    void on_pushButton_chatwindowBack_clicked();

    void on_sendButton_clicked();

private:
    Ui::ChatWindow *ui;
    QString m_username;
    INetworkClient* m_network;
};

#endif // CHATWINDOW_H
