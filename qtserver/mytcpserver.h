#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QtCore>
#include <QtNetwork>

#include "packettypes.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

signals:
    bool dataReceived(QByteArray);

public slots:
    void newConnection();
    void disconnected();
    void readyRead();
    bool writeData(QByteArray);

public:
    QByteArray data;
    QTcpServer *server;
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely
    QByteArray test {"test_qstring"};
    //unsigned int count {0};
    float count {0};
    float factor {1};
    float position_temp[19];
};

#endif // MYTCPSERVER_H
