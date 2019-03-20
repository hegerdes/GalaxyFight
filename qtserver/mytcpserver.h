#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QtCore>
#include <QtNetwork>
#include "../math/Vector.hpp"
#include "flags.h"

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
    //float count {0};
    //float factor {1};
    float position_temp[12];
    float position_temp_2[12];

    QTcpSocket *socket_1 = nullptr;
    QTcpSocket *socket_2 = nullptr;
    float getFloat(char** ptr);
    short getShort(char** ptr);
    char getChar(char** ptr);
    int getInt(char** ptr);


    struct client_data{
        asteroids::Vector<float> position;
        asteroids::Vector<float> xAxis;
        asteroids::Vector<float> yAxis;
        asteroids::Vector<float> zAxis;
        Bullet_shot shot;
        int bullet_id;
        Living living;
    };

    client_data user_data_1;
    client_data user_data_2;
};

#endif // MYTCPSERVER_H
