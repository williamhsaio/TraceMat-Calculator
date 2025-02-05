#include "mainwindow.h"
#include "connection.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createConnection()){
        return 1;
    }
    MainWindow w;
    w.show();
    int r = a.exec();
    closeConnection();
    return r;
}
