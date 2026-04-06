#include "groupwindow.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // MainWindow w;
    //w.show();
    GroupWindow w2;
    w2.show();
    return a.exec();
}
