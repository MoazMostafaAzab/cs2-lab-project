#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "INetworkClient.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(INetworkClient* network, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onLoginSuccess(const QString& username);


private:
    Ui::MainWindow *ui;
    INetworkClient* m_network;
};

#endif // MAINWINDOW_H
