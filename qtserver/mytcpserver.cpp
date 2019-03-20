// mytcpserver.cpp

#include "mytcpserver.h"
#include <iostream>

static inline QByteArray IntToArray(qint32 source);
static inline qint32 ArrayToInt(QByteArray source);

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    qDebug() << "Listening:" << server->listen(QHostAddress::Any, 38291);
    std::cout << server->serverAddress().toString().toUtf8().constData() << "; serverAdress\n";
    std::cout << server->serverPort() << "; serverPort\n";
    //qDebug() << "Listening:" << server->listen(QHostAddress::LocalHost, 1024);
}

void Server::newConnection()
{
    qDebug() << "Listening:";
    std::cout << "new conneciton\n";
    while (server->hasPendingConnections())
    {
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

        if(socket_1 == nullptr){
            socket_1 = socket;
        }else if(socket_2 == nullptr){
            socket_2 = socket;
        }else{
            socket->abort();
        }

        //connect(this, SIGNAL(dataReceived(QByteArray)),
                //this, SLOT(writeData(QByteArray)));

        QByteArray *buffer = new QByteArray();
        qint32 *s = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, s);
        std::cout << "successful connect\n";
    }
}

void Server::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    if(socket_1 == socket){
        socket_1 = nullptr;
    }else if(socket_2 == socket){
        socket_2 = nullptr;
    }else{
        //ERROR
    }

    std::cout << "disconnect success\n";
    socket->deleteLater();
    delete buffer;
    delete s;
    socket->flush();
}

bool Server::writeData(QByteArray)
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray message;

    if(socket_1 == socket) {
        message.append((char*)&position_temp_2[0],(4*12));
    } else if(socket_2 == socket) {
        message.append((char*)&position_temp[0],(4*12));
    } else {
        //ERROR
    }

    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(message); //write the data itself
        return socket->waitForBytesWritten();
    }
    else{
        std::cerr << socket->state() << ": connected State\n";
        return false;
    }

}

void Server::readyRead()
{
    //std::cout << "read success\n";
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    qint32 size = *s;
    while (socket->bytesAvailable() > 0)
    {
        buffer->append(socket->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
        {
            if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
            {
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;


                //Interprete the packet-type:
                //PacketType type = data[0];

                //get packet and
                float* temp = (float*) data.data();

                for(int i {0}; i < 12; i++){
                    if(socket_1 == socket){
                        position_temp[i] = temp[i];
                    }else if(socket_2 == socket){
                        position_temp_2[i] = temp[i];
                    }else{
                        //ERROR
                    }
                }

                this->writeData(data);
            }
        }
    }
}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
