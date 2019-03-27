// mytcpserver.cpp

#include "mytcpserver.h"
#include <iostream>


static inline QByteArray IntToArray(qint32 source);
static inline qint32 ArrayToInt(QByteArray source);

Server::Server(QObject* parent) : QObject(parent), physics(user_data_1, user_data_2) {
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    bool connected = server->listen(QHostAddress::Any, 38292);
    if(connected){
        log(LoggingType::INFO, "Listening on Address: " + server->serverAddress().toString().toStdString());
        int port = server->serverPort();
        log(LoggingType::INFO, "Listening on Port: " + std::to_string(port));
    }else{
        log(LoggingType::ERROR, "Could not bind to port!");
        exit(0);
    }

    user_data_1.position = {-650, 0, 0};
    user_data_1.xAxis[0] = -1;
    user_data_1.yAxis[1] = -1;
    user_data_1.zAxis[2] = 1;
    user_data_1.shot = Bullet_shot::not_shot;
    user_data_2.position = {0, 0, 0};
    user_data_2.xAxis[0] = 1;
    user_data_2.yAxis[1] = 1;
    user_data_2.zAxis[2] = 1;
    user_data_2.shot = Bullet_shot::not_shot;

    // generate asteroids:
    int amount = 10;
    for (int i{0}; i < amount; i++) {
        pos_astr[i] = asteroids::Randomizer::instance()->getRandomVertex(1000);
        float rand = asteroids::Randomizer::instance()->getRandomNumber(0, 0.1);
        dir_astr[i] = asteroids::Randomizer::instance()->getRandomVertex(1.0) * rand;
        size_astr[i] = asteroids::Randomizer::instance()->getRandomNumber(0, 100);

        physics.addAsteroid(asteroids::ServerAsteroid::Ptr(new asteroids::ServerAsteroid(pos_astr[i], dir_astr[i], 1, size_astr[i], i)));
    }
}

void Server::newConnection() {
    //qDebug() << "Listening:\n ";
    //std::cout << "new conneciton" << std::endl;

    while (server->hasPendingConnections()) {
        QTcpSocket* socket = server->nextPendingConnection();
        log(LoggingType::INFO, "Client try to connect on Address: " + socket->peerAddress().toString().toStdString());

        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

        if (socket_1 == nullptr) {
            socket_1 = socket;
        } else if (socket_2 == nullptr) {
            socket_2 = socket;
        } else {
            socket->abort();
        }

        // connect(this, SIGNAL(dataReceived(QByteArray const &)),
        // this, SLOT(writeData(QByteArray const &)));

        QByteArray* buffer = new QByteArray();
        qint32* s = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, s);
        //std::cout << "successful connect" << std::endl;
        log(LoggingType::INFO, "Client connected successful on Address: " + socket->peerAddress().toString().toStdString());
    }
}

void Server::disconnected() {
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QByteArray* buffer = buffers.value(socket);
    qint32* s = sizes.value(socket);
    if (socket_1 == socket) {
        socket_1 = nullptr;
    } else if (socket_2 == socket) {
        socket_2 = nullptr;
    } else {
        // ERROR
    }
    already_send_1 = false;
    already_send_2 = false;

    //std::cout << "disconnect success\n";
    log(LoggingType::INFO, "Client disconnected on Address: " + socket->peerAddress().toString().toStdString());
    socket->deleteLater();
    delete buffer;
    delete s;
    socket->flush();
}

bool Server::writeData(QByteArray const& data) {
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    QByteArray response;

    PacketType toSend = PacketType::update_3D_C;

    //to remove
    if (socket_1 != nullptr && socket_2 != nullptr && !already_send_1 && socket_1 == socket) {
        toSend = PacketType::init_3D;
        already_send_1 = true;
        physics.run();
    } else if (socket_1 != nullptr && socket_2 != nullptr && !already_send_2 && socket_2 == socket) {
        toSend = PacketType::init_3D;
        already_send_2 = true;
    }

    //determine which packet has to be send
    if (toSend == PacketType::update_3D_C) {
        sendUpdate_3D_S(response, socket);
    } else if (toSend == PacketType::init_3D) {
        sendInit_3D(response, socket);
    } else if(toSend == PacketType::game_start){
        sendGame_Start(response, socket);
    }

    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(response); // write the data itself
        return socket->waitForBytesWritten();
    } else {
        log(LoggingType::INFO, "Client not connected on Address: " + socket->peerAddress().toString().toStdString());
        //std::cerr << socket->state() << ": connected State\n";
        return false;
    }
}

void Server::sendUpdate_3D_S(QByteArray& response, QTcpSocket* socket) {
    client_data client_data_temp;
    if (socket_1 == socket) {
        client_data_temp = user_data_2;
        //std::cerr << "Socket send 2";
    } else if (socket_2 == socket) {
        client_data_temp = user_data_1;
        //std::cerr << "Socket send 1";
    } else {
        //std::cerr << "client socket not recognized\n";
        log(LoggingType::ERROR, "Client not recognized: " + socket->peerAddress().toString().toStdString());
    }

    response.append(PacketType::update_3D_S);
    response.append((char*) &client_data_temp.position, 3 * 4);
    response.append((char*) &client_data_temp.xAxis, 3 * 4);
    response.append((char*) &client_data_temp.yAxis, 3 * 4);
    response.append((char*) &client_data_temp.zAxis, 3 * 4);

    std::vector<int> ids_astr;

    if(socket == socket_1){
        ids_astr = physics.getDelAstr1();
    }else{
        ids_astr = physics.getDelAstr2();
    }

    short length = ids_astr.size();
    response.append((char*)&length, 2); // anzahl zerstörter asteroidend short eine shor null
    for(auto it : ids_astr){
        response.append((char*)&it, 4);
    }
    // client_data_temp.deleted_asteroids_id.clear();
    // Asteroiden ids wenn nötig > 0

    response.append(client_data_temp.shot);
    response.append((char*) &client_data_temp.bullet_id, 4);

    //RIGHT?????????????????????????????????????????
    response.append(Hit::hit);

    //BULLETS
    std::vector<int> ids;

    if(socket == socket_1){
        ids = physics.getDelBullets1();
    }else{
        ids = physics.getDelBullets2();
    }

    int b_length = ids.size();
    response.append((char*)&b_length, 4); // anzahl zerstörter asteroidend short eine shor null

    for(auto it : ids){
        response.append((char*)&it, 4);
        log(LoggingType::DEBUG, "Bullet removed SEND TO CLIENT: " + std::to_string(it));
    }
    //client_data_temp.deleted_bullets_id.clear();
    // Bullet ids wenn nötig > 0

    if(socket_1 == socket){
        int own = physics.getFirstHealth();
        response.append((char*)&own, 4);
        int enemy = physics.getSecondHealth();
        response.append((char*)&enemy, 4);
    }else{
        int own = physics.getSecondHealth();
        response.append((char*)&own, 4);
        int enemy = physics.getFirstHealth();
        response.append((char*)&enemy, 4);
    }

    //update bullets
    int changed_count = physics.m_first_bullets.size() + physics.m_second_bullets.size();
    response.append((char*)&changed_count, 4);
    for(auto it : physics.m_first_bullets){
        response.append(it->m_id);
        response.append((char*) &it->m_pos, 3 * 4);
        response.append((char*) &it->m_dir, 3 * 4);
    }
    for(auto it : physics.m_second_bullets){
        response.append(it->m_id);
        response.append((char*) &it->m_pos, 3 * 4);
        response.append((char*) &it->m_dir, 3 * 4);
    }
    //update astr
    int astr_count = physics.m_asteroids.size();
    response.append((char*)&astr_count, 4);
    for(auto it : physics.m_asteroids){
        response.append(it->m_id);
        response.append((char*) &it->m_pos, 3 * 4);
    }
}

void Server::recvUpdate_3D_C(char* data, QTcpSocket* socket) {
    client_data client_data_temp;
    client_data_temp.position[0] = getFloat(&data);
    client_data_temp.position[1] = getFloat(&data);
    client_data_temp.position[2] = getFloat(&data);

    // std::cout << client_data_temp.position << std::endl;

    client_data_temp.xAxis[0] = getFloat(&data);
    client_data_temp.xAxis[1] = getFloat(&data);
    client_data_temp.xAxis[2] = getFloat(&data);

    // std::cout << client_data_temp.xAxis << std::endl;

    client_data_temp.yAxis[0] = getFloat(&data);
    client_data_temp.yAxis[1] = getFloat(&data);
    client_data_temp.yAxis[2] = getFloat(&data);

    // std::cout << client_data_temp.yAxis << std::endl;

    client_data_temp.zAxis[0] = getFloat(&data);
    client_data_temp.zAxis[1] = getFloat(&data);
    client_data_temp.zAxis[2] = getFloat(&data);

    // std::cout << client_data_temp.zAxis << std::endl;

    client_data_temp.shot = (Bullet_shot) getChar(&data);
    client_data_temp.bullet_id = getInt(&data);
    if(client_data_temp.shot == Bullet_shot::shot){
        asteroids::Vector3f shipPosition = client_data_temp.position + client_data_temp.zAxis * -45 + client_data_temp.xAxis * -175;
        physics.addBullet(asteroids::ServerBullet::Ptr(new asteroids::ServerBullet(shipPosition, client_data_temp.xAxis * -1, client_data_temp.bullet_id)));
        log(LoggingType::DEBUG, "Send Bullet ID: " + std::to_string(client_data_temp.bullet_id));
    }

    client_data_temp.living = (Living) getChar(&data);

    if (socket_1 == socket) {
        user_data_1 = client_data_temp;
    } else if (socket_2 == socket) {
        user_data_2 = client_data_temp;
    } else {
        //std::cerr << "client socket not recognized\n";
        log(LoggingType::ERROR, "Client not recognized: " + socket->peerAddress().toString().toStdString());
    }
}

void Server::recvReady_T(char* data, QTcpSocket* socket) {
    int length = getInt(&data);
    char id[length + 1];
    for(int i = 0; i < length; i++)
    {
        id[i] = getChar(&data);
    }
    id[length] = '\0';

    std::string name = std::string(id);

    if (socket_1 == socket) {
        user_data_1.name = name;
    } else if (socket_2 == socket) {
        user_data_2.name = name;
    } else {
        //std::cerr << "client socket not recognized\n";
        log(LoggingType::ERROR, "Client not recognized: " + socket->peerAddress().toString().toStdString());
    }

}

void Server::sendInit_3D(QByteArray& response, QTcpSocket* socket) {
    client_data client_data_temp_own;
    client_data client_data_temp_enemy;
    if (socket_1 == socket) {
        client_data_temp_own = user_data_1;
        client_data_temp_enemy = user_data_2;
    } else if (socket_2 == socket) {
        client_data_temp_own = user_data_2;
        client_data_temp_enemy = user_data_1;
    } else {
        //std::cerr << "client socket not recognized\n";
        log(LoggingType::ERROR, "Client not recognized: " + socket->peerAddress().toString().toStdString());
    }

    response.append(PacketType::init_3D);

    response.append((char*) &client_data_temp_own.position, 12);
    response.append((char*) &client_data_temp_own.xAxis, 12);
    response.append((char*) &client_data_temp_own.yAxis, 12);
    response.append((char*) &client_data_temp_own.zAxis, 12);

    response.append((char*) &client_data_temp_enemy.position, 12);
    response.append((char*) &client_data_temp_enemy.xAxis, 12);
    response.append((char*) &client_data_temp_enemy.yAxis, 12);
    response.append((char*) &client_data_temp_enemy.zAxis, 12);

    // Astroids:
    int amount = 10;
    response.append((char*) &amount, 4);

    for (int i = 0; i < amount; i++) {
        response.append((char*) &i, 4);

        asteroids::Vector3f tmp = pos_astr[i];
        response.append((char*) &tmp, 12);

        tmp = dir_astr[i];
        response.append((char*) &tmp, 12);

        float size = size_astr[i];
        response.append((char*) &size, 4);
    }
    if (socket_1 == socket) {
        response.append(player_no::first);
    } else if (socket_2 == socket) {
        response.append(player_no::second);
    }
}

void Server::sendGame_Start(QByteArray& response, QTcpSocket* socket) {
    client_data client_data_temp_own;
    client_data client_data_temp_enemy;
    if (socket_1 == socket) {
        client_data_temp_own = user_data_1;
        client_data_temp_enemy = user_data_2;
    } else if (socket_2 == socket) {
        client_data_temp_own = user_data_2;
        client_data_temp_enemy = user_data_1;
    } else {
        //std::cerr << "client socket not recognized\n";
        log(LoggingType::ERROR, "Client not recognized: " + socket->peerAddress().toString().toStdString());
    }

    response.append(PacketType::game_start);

    int length = client_data_temp_enemy.name.length();

    response.append((char*)&length, 4);
    response.append(client_data_temp_enemy.name.c_str(), length);
    if (socket_1 == socket) {
        response.append(player_no::first);
    } else if (socket_2 == socket) {
        response.append(player_no::second);
    }

    //TODO send fist map config data
}

void Server::log(LoggingType type, std::string msg){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    std::string str(buffer);

    std::ofstream logfile;
    logfile.open ("log.txt");
    if(type == LoggingType::ERROR){
        std::cerr << "[" << str << " ERROR] " << msg << "\n";
        logfile << "[" << str << " ERROR] " << msg << "\n";
    }else if(type == LoggingType::INFO){
        std::cout << "[" << str << " INFO] " << msg << std::endl;
        logfile << "[" << str << " INFO] " << msg << std::endl;
    }else if(type == LoggingType::DEBUG){
        std::cout << "[" << str << " DEBUG] " << msg << std::endl;
        logfile << "[" << str << " DEBUG] " << msg << std::endl;
    }
    logfile.close();
}

void Server::readyRead() {
    // std::cout << "read success\n";
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QByteArray* buffer = buffers.value(socket);
    qint32* s = sizes.value(socket);
    qint32 size = *s;
    while (socket->bytesAvailable() > 0) {
        buffer->append(socket->readAll());
        while ((size == 0 && buffer->size() >= 4) ||
               (size > 0 && buffer->size() >= size)) // While can process data, process it
        {
            if (size == 0 && buffer->size() >= 4) // if size of data has received completely, then
                                                  // store it on our global variable
            {
                size = ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            // If data has received completely, then emit
            // our SIGNAL with the data
            if (size > 0 && buffer->size() >= size)

            {
                QByteArray answer = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;

                // Interprete the packet-type:
                char* data = (char*) answer.data();
                PacketType pt = (PacketType) getChar(&data);
                ////std::cout << pt << std::endl;

                //already send / to be removed TODO
                if (pt == PacketType::update_3D_C && already_send_1 && already_send_2) {
                    recvUpdate_3D_C(data, socket);
                } else if (pt == PacketType::ready_T) {
                    recvReady_T(data, socket);
                }

                // direct response after receiving
                this->writeData(nullptr);
            }
        }
    }
}

float Server::getFloat(char** ptr) {
    float* jo = (float*) *ptr;
    float f = *jo;
    *ptr += 4;
    return f;
}

short Server::getShort(char** ptr) {
    short* jo = (short*) *ptr;
    short f = *jo;
    *ptr += 2;
    return f;
}

char Server::getChar(char** ptr) {
    char f = **ptr;
    *ptr += 1;
    return f;
}

int Server::getInt(char** ptr) {
    int* jo = (int*) *ptr;
    int f = *jo;
    *ptr += 4;
    return f;
}

qint32 ArrayToInt(QByteArray source) {
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

QByteArray IntToArray(qint32 source) // Use qint32 to ensure that the number have 4 bytes
{
    // Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
