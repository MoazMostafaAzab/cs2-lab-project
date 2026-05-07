#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "chatmanager.h"
#include "NetworkClient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onLoginSuccess(const QString& username);

private:
    Ui::MainWindow *ui;
    NetworkClient* m_network = nullptr;
    ChatManager* m_chatManager = nullptr;
};
#endif // MAINWINDOW_H
