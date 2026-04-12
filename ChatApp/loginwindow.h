 #ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "LoginManager.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(LoginManager* manager, QWidget* parent = nullptr);
    ~LoginWindow();
signals:
    void loginSuccessful(const QString& username);

private slots:
    void on_connectButton_clicked();

private:
    Ui::LoginWindow *ui;
    LoginManager* m_manager;
};

#endif // LOGINWINDOW_H
