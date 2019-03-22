#include <QCoreApplication>
#include "mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // create MyTcpServer
    // MyTcpServer constructor will create QTcpServer

    Server server;

    return a.exec();
}
