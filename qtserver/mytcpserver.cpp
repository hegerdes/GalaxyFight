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
    user_data_1.position = {-650,0,0};
    user_data_1.xAxis[0] = -1;
    user_data_1.yAxis[1] = -1;
    user_data_1.zAxis[2] = 1;
    user_data_1.shot = Bullet_shot::not_shot;
    user_data_2.position = {0,0,0};
    user_data_2.xAxis[0] = 1;
    user_data_2.yAxis[1] = 1;
    user_data_2.zAxis[2] = 1;
    user_data_1.shot = Bullet_shot::not_shot;

    //generate asteroids:
    int amount = 10;
    for(int i{0}; i < amount; i++)
    {
        pos_astr[i] = asteroids::Randomizer::instance()->getRandomVertex(1000);
        dir_astr[i] = asteroids::Randomizer::instance()->getRandomVertex(1.0);
        size_astr[i] = asteroids::Randomizer::instance()->getRandomNumber(0, 100);
    }
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

        //connect(this, SIGNAL(dataReceived(QByteArray const &)),
                //this, SLOT(writeData(QByteArray const &)));

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
    already_send_1 = false;
    already_send_2 = false;

    std::cout << "disconnect success\n";
    socket->deleteLater();
    delete buffer;
    delete s;
    socket->flush();
}

bool Server::writeData(QByteArray const & data)
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());

    QByteArray response;

    PacketType toSend = PacketType::update_3D_C;

    if(socket_1 != nullptr && socket_2 != nullptr && !already_send_1 && socket_1 == socket){
        toSend = PacketType::init_3D;
        already_send_1 = true;
    }else if(socket_1 != nullptr && socket_2 != nullptr && !already_send_2 && socket_2 == socket){
        toSend = PacketType::init_3D;
        already_send_2 = true;
    }

    if(toSend == PacketType::update_3D_C){

        client_data client_data_temp;
        if(socket_1 == socket){
           client_data_temp = user_data_2;
        }else if(socket_2 == socket){
           client_data_temp = user_data_1;
        }else{
            std::cerr << "client socket not recognized\n";
        }

        response.append(PacketType::update_3D_S);
        //std::cout << "Packettype out: " << response.data()[0] << std::endl;
        //std::cout << "----------------" << std::endl;

        response.append((char*)&client_data_temp.position, 3*4);
        //std::cout << client_data_temp.position << std::endl;

        response.append((char*)&client_data_temp.xAxis, 3*4);
        //std::cout << client_data_temp.xAxis << std::endl;

        response.append((char*)&client_data_temp.yAxis, 3*4);
        //std::cout << client_data_temp.yAxis << std::endl;

        response.append((char*)&client_data_temp.zAxis, 3*4);
        //std::cout << client_data_temp.zAxis << std::endl;

        response.append(2, char{}); // anzahl zerstörter asteroidend short eine shor null
        // Asteroiden ids wenn nötig > 0
        response.append(client_data_temp.shot);
        response.append((char*)&client_data_temp.bullet_id, 4);
        response.append(Hit::hit);
        response.append(4, char{}); // zerstörte bullets
        // Bullet ids wenn nötig > 0
    } else if(toSend == PacketType::init_3D){
        client_data client_data_temp_own;
        client_data client_data_temp_enemy;
        if(socket_1 == socket){
           client_data_temp_own = user_data_1;
           client_data_temp_enemy = user_data_2;
        }else if(socket_2 == socket){
            client_data_temp_own = user_data_2;
            client_data_temp_enemy = user_data_1;
        }else{
            std::cerr << "client socket not recognized\n";
        }

        std::cerr << "init send" << std::endl;

        std::cerr << "try to send asddsasda?????\n";

        response.append(PacketType::init_3D);

        std::cerr << "try to send astr234432\n";

        response.append((char*)&client_data_temp_own.position, 12);
        response.append((char*)&client_data_temp_own.xAxis, 12);
        response.append((char*)&client_data_temp_own.yAxis, 12);
        response.append((char*)&client_data_temp_own.zAxis, 12);

        std::cerr << "try to send astr222\n";

        response.append((char*)&client_data_temp_enemy.position, 12);
        response.append((char*)&client_data_temp_enemy.xAxis, 12);
        response.append((char*)&client_data_temp_enemy.yAxis, 12);
        response.append((char*)&client_data_temp_enemy.zAxis, 12);


        std::cerr << "try to send astr1111\n";

        //Astroids:
        int amount = 10;
        response.append((char*)&amount, 4);

        std::cerr << "try to send astr\n";
        for(int i = 0; i < amount; i++)
        {
            response.append((char*)&i, 4);

            asteroids::Vector3f tmp = pos_astr[i];
            response.append((char*)&tmp, 12);

            tmp = dir_astr[i];
            response.append((char*)&tmp, 12);

            float size = size_astr[i];
            response.append((char*)&size, 4);

            std::cerr << i << ". Asteroid verpackt!" << "\n";
        }

    }

    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(response); //write the data itself
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
                QByteArray answer = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;


                //Interprete the packet-type:
                char* data = (char*) answer.data();
                PacketType pt = (PacketType) getChar(&data);
                ////std::cout << pt << std::endl;

                if(pt == PacketType::update_3D_C && already_send_1 && already_send_2){
                    client_data client_data_temp;
                    client_data_temp.position[0] = getFloat(&data);
                    client_data_temp.position[1] = getFloat(&data);
                    client_data_temp.position[2] = getFloat(&data);

                    //std::cout << client_data_temp.position << std::endl;

                    client_data_temp.xAxis[0] = getFloat(&data);
                    client_data_temp.xAxis[1] = getFloat(&data);
                    client_data_temp.xAxis[2] = getFloat(&data);

                    //std::cout << client_data_temp.xAxis << std::endl;

                    client_data_temp.yAxis[0] = getFloat(&data);
                    client_data_temp.yAxis[1] = getFloat(&data);
                    client_data_temp.yAxis[2] = getFloat(&data);

                    //std::cout << client_data_temp.yAxis << std::endl;

                    client_data_temp.zAxis[0] = getFloat(&data);
                    client_data_temp.zAxis[1] = getFloat(&data);
                    client_data_temp.zAxis[2] = getFloat(&data);

                    //std::cout << client_data_temp.zAxis << std::endl;

                    client_data_temp.shot = (Bullet_shot) getChar(&data);
                    client_data_temp.bullet_id = getInt(&data);
                    client_data_temp.living = (Living) getChar(&data);

                    if(socket_1 == socket){
                       user_data_1 = client_data_temp;
                    }else if(socket_2 == socket){
                       user_data_2 = client_data_temp;
                    }else{
                        std::cerr << "client socket not recognized\n";
                    }
                }




                //PacketType type = data[0];
                //get packet and
                /*
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
                */

                this->writeData(nullptr);
            }
        }
    }
}

float Server::getFloat(char** ptr){
    float * jo = (float*)*ptr;
    float f = *jo;
    *ptr += 4;
    return f;
}

short Server::getShort(char** ptr){
    short * jo = (short*)*ptr;
    short f = *jo;
    *ptr += 2;
    return f;
}

char Server::getChar(char** ptr){
    char f = **ptr;
    *ptr += 1;
    return f;
}

int Server::getInt(char** ptr){
    int * jo = (int*)*ptr;
    int f = *jo;
    *ptr += 4;
    return f;
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
