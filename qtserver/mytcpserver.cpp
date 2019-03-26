// mytcpserver.cpp

#include "mytcpserver.h"
#include <iostream>

static inline QByteArray IntToArray(qint32 source);
static inline qint32 ArrayToInt(QByteArray source);

Server::Server(QObject* parent) : QObject(parent) {
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

    user_data_1.position = {-100, 0, 0};
    user_data_1.xAxis[0] = -1;
    user_data_1.yAxis[1] = -1;
    user_data_1.zAxis[2] = 1;
    user_data_1.shot = Bullet_shot::not_shot;
    user_data_2.position = {100, -0, -0};
    user_data_2.xAxis[0] = 1;
    user_data_2.yAxis[1] = 1;
    user_data_2.zAxis[2] = 1;
    user_data_2.shot = Bullet_shot::not_shot;

    // generate asteroids:
    int amount = 10;
    for (int i{0}; i < amount; i++) {
        pos_astr[i] = asteroids::Randomizer::instance()->getRandomVertex(1000);
        dir_astr[i] = asteroids::Randomizer::instance()->getRandomVertex(1.0);
        size_astr[i] = asteroids::Randomizer::instance()->getRandomNumber(0, 100);
    }
    pchanges_size1 = -1;
    pchanges_size2 = -1;
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
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << " packet received sucevfully \n";
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    QByteArray response;

    /*
    if(PacketType::update_3D_C == toSend){
        std::cerr << __LINE__ << "\n";
    } else */if( !user_data_1.m_first_connect || !user_data_2.m_first_connect)
    {
        std::cerr << __LINE__ << "\n";
        return false;
    }

    std::cerr << __LINE__ << "\n";
    //determine which packet has to be send
    if (toSend == PacketType::update_3D_C) {
        std::cerr << __LINE__ << "\n";
        sendUpdate_3D_S(response, socket);
    } else if(send_changes) {
        std::cerr << __LINE__ << "\n";
        toSend = PacketType::planet_changes2d;
        send_changes = false;
    }
    else if (toSend == PacketType::init_3D) {
        //std::cerr << __LINE__ << "\n";
        //sendInit_3D(response, socket);
        m_player1_end_3d_received = false;
        user_data_1.position = {-100, 0, 0};
        user_data_1.xAxis[0] = -1;
        user_data_1.yAxis[1] = -1;
        user_data_1.zAxis[2] = 1;
        user_data_1.shot = Bullet_shot::not_shot;
        user_data_2.position = {100, -0, -0};
        user_data_2.xAxis[0] = 1;
        user_data_2.yAxis[1] = 1;
        user_data_2.zAxis[2] = 1;
        user_data_2.shot = Bullet_shot::not_shot;
        std::cerr << __LINE__ << "\n";
        bool bytes_written_1 = false;
        sendInit_3D(response, socket_1);
        if (socket_1->state() == QAbstractSocket::ConnectedState) {
        std::cerr << __LINE__ << "\n";
            socket_1->write(response); // write the data itself
            bytes_written_1 = socket_1->waitForBytesWritten();
        } else {
            log(LoggingType::INFO, "Client not connected on Address: " + socket->peerAddress().toString().toStdString());
        }

        bool bytes_written_2 = false;
        QByteArray response_2;
        sendInit_3D(response_2, socket_2);
        if (socket_2->state() == QAbstractSocket::ConnectedState) {
        std::cerr << __LINE__ << "\n";
            socket_2->write(response_2); // write the data itself
            bytes_written_2 = socket_2->waitForBytesWritten();
        } else {
            log(LoggingType::INFO, "Client not connected on Address: " + socket->peerAddress().toString().toStdString());
        }

        //return bytes_written_1 && bytes_written_2;
        std::cerr << __LINE__ << " written both init_3d_packets\n";
        return true;
    } else if(toSend == PacketType::game_start){
        std::cerr << __LINE__ << "\n";
        bool bytes_written_1 = false;
        sendGame_Start(response, socket_1);
        if (socket_1->state() == QAbstractSocket::ConnectedState) {
        std::cerr << __LINE__ << "\n";
            socket_1->write(response); // write the data itself
            bytes_written_1 = socket_1->waitForBytesWritten();
        } else {
            log(LoggingType::INFO, "Client not connected on Address: " + socket->peerAddress().toString().toStdString());
        }

        bool bytes_written_2 = false;
        QByteArray response_2;
        sendGame_Start(response_2, socket_2);
        if (socket_2->state() == QAbstractSocket::ConnectedState) {
        std::cerr << __LINE__ << "\n";
            socket_2->write(response_2); // write the data itself
            bytes_written_2 = socket_2->waitForBytesWritten();
        } else {
            log(LoggingType::INFO, "Client not connected on Address: " + socket->peerAddress().toString().toStdString());
        }

        //return bytes_written_1 && bytes_written_2;
        std::cerr << __LINE__ << " written both game_start_packets\n";
        return true;
    }

    if (socket->state() == QAbstractSocket::ConnectedState) {
        std::cerr << __LINE__ << "\n";
        socket->write(response); // write the data itself
        return socket->waitForBytesWritten();
    } else {
        log(LoggingType::INFO, "Client not connected on Address: " + socket->peerAddress().toString().toStdString());
        //std::cerr << socket->state() << ": connected State\n";
        return false;
    }
    std::cerr << "\n";
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

    response.append(2, char{}); // anzahl zerstörter asteroidend short eine shor null
    // Asteroiden ids wenn nötig > 0

    response.append(client_data_temp.shot);
    response.append((char*) &client_data_temp.bullet_id, 4);
    response.append(Hit::hit);
    response.append(4, char{}); // zerstörte bullets
    // Bullet ids wenn nötig > 0
}

void Server::recvUpdate_3D_C(char* data, QTcpSocket* socket) {
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
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
    client_data_temp.living = (Living) getChar(&data);
    client_data_temp.bullet_id = getInt(&data);

    if (socket_1 == socket) {
        user_data_1 = client_data_temp;
        user_data_1.m_first_connect = true;
    } else if (socket_2 == socket) {
        user_data_2 = client_data_temp;
        user_data_2.m_first_connect = true;
    } else {
        //std::cerr << "client socket not recognized\n";
        log(LoggingType::ERROR, "Client not recognized: " + socket->peerAddress().toString().toStdString());
    }
    std::cerr << "\n";
}

void Server::sendEnd3D(QByteArray& data, QTcpSocket* socket){

    data.append(PacketType::end_3D);
    data.append((char*)&winner, 4);

}

void Server::recvReady_T(char* data, QTcpSocket* socket) {
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << " packet received sucevfully \n";
    int length = getInt(&data);
    char id[length + 1];
    for(int i = 0; i < length; i++)
    {
        id[i] = getChar(&data);
    }
    id[length] = '\0';

    std::string name = std::string(id);

    if (socket_1 == socket) {
        std::cerr << __LINE__ << "\n";
        user_data_1.name = name;
        user_data_1.m_first_connect = true;
    } else if (socket_2 == socket) {
        std::cerr << __LINE__ << "\n";
        user_data_2.name = name;
        user_data_2.m_first_connect = true;
    } else {
        //std::cerr << "client socket not recognized\n";
        log(LoggingType::ERROR, "Client not recognized: " + socket->peerAddress().toString().toStdString());
    }
    std::cerr << "\n";
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
    }
    logfile.close();
}

void Server::readyRead() {
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
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
                if(pt == PacketType::planet_changes2d)
                {
                    if(player1_outstanding_fights.empty()){
                    std::cerr << __LINE__ << "\n";
                        recvPlanetChanges(data, socket);
                    } else {
                    std::cerr << __LINE__ << "\n";
                        toSend = PacketType::init_3D;
                        this->writeData(nullptr);
                    }
                //} else if (pt == PacketType::update_3D_C && already_send_1 && already_send_2) {
                } else if (pt == PacketType::update_3D_C) {
                    std::cerr << __LINE__ << "\n";
                    recvUpdate_3D_C(data, socket);
                    toSend = PacketType::update_3D_C;
                    this->writeData(nullptr);
                } else if (pt == PacketType::reset_planet_changes) {
                    if(socket_1 == socket)
                    {
                        m_socket_1_pchange_received = false;
                    }
                    if(socket_2 == socket)
                    {
                        m_socket_2_pchange_received = false;
                    }
                } else if (pt == PacketType::end_3D) {
                    recv_end_3D(data, socket);
                    if(player1_outstanding_fights.empty())
                    {
                        std::cerr << __LINE__ << "_____________________________________________________\n";
                        sendUpdatedPlanetChanges();
                    } else {
                        std::cerr << __LINE__ << "_____________________________________________________\n";
                        toSend = PacketType::init_3D;
                        this->writeData(nullptr);
                    }
                } else if (pt == PacketType::ready_T) {
                    std::cerr << __LINE__ << "\n";
                    recvReady_T(data, socket);
                    toSend = PacketType::game_start;
                    this->writeData(nullptr);
                } else {
                    toSend=pt;
                    std::cerr << __LINE__ << ", packet_type: " << pt << "\n";
                    this->writeData(nullptr);
                }


                // direct response after receiving
            }
        }
    }
    std::cerr << "end\n";
}

void Server::recv_end_3D(char* data, QTcpSocket* socket){
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << " packet received sucevfully \n";
    player_no winner_loser = (player_no) getChar(&data);
    player_no player_no_temp = (player_no) getChar(&data);
    std::cerr << __LINE__ << ", player_no" << player_no_temp << ", winner_loser: " << winner_loser << "\n";
    //if(!m_player1_end_3d_received && (player_no::winner == winner_loser)){
    if((player_no::winner == winner_loser)){
        std::cerr << __LINE__ << "\n";
        if(player_no_temp == player_no::first)
        {
            player1_outstanding_fights.front().m_own = asteroids::PlanetChanges::Owner::PLAYER1;
            pchanges_committ.push_back(player1_outstanding_fights.front());
        } else {
            player2_outstanding_fights.front().m_own = asteroids::PlanetChanges::Owner::PLAYER2;
            pchanges_committ.push_back(player2_outstanding_fights.front());
        }
        auto it1 = player1_outstanding_fights.begin();
        player1_outstanding_fights.erase(it1);
        auto it2 = player2_outstanding_fights.begin();
        player2_outstanding_fights.erase(it2);
        m_player1_end_3d_received = true;
    } else {
        /*
        std::cerr << __LINE__ << "\n";
        if(player_no_temp == player_no::first)
        {
            pchanges_committ.push_back(player2_outstanding_fights.front());
        } else {
            pchanges_committ.push_back(player1_outstanding_fights.front());
        }
        auto it1 = player1_outstanding_fights.begin();
        player1_outstanding_fights.erase(it1);
        auto it2 = player2_outstanding_fights.begin();
        player2_outstanding_fights.erase(it2);
        m_player1_end_3d_received = true;
        */
    }
        std::cerr << __LINE__ << "\n";
}

void Server::recvPlanetChanges(char* data, QTcpSocket* socket)
{
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << " packet received sucevfully \n";
    bool this_socket_already_received = false;
    if(socket_1 == socket)
    {
        std::cerr << __LINE__ << "\n";
        if( m_socket_1_pchange_received) {
            this_socket_already_received = true;
        }
    }
    if(socket_2 == socket)
    {
        std::cerr << __LINE__ << "\n";
        if( m_socket_2_pchange_received) {
            this_socket_already_received = true;
        }
    }
    if(!this_socket_already_received){
        int size = getInt(&data);
        for(int i = 0 ; i < size ; i++)
        {
            pchanges_data tmp;
            tmp.m_own = (asteroids::PlanetChanges::Owner)getChar(&data);
            tmp.m_id = getInt(&data);
            tmp.m_num_of_ore =getInt(&data) ;
            tmp.num_factory = getInt(&data);
            tmp.num_mine = getInt(&data);
            tmp.num_fighters = getInt(&data);
            tmp.num_transporter = getInt(&data);
            char m_attack_planet  = getChar(&data);

            std::cerr << __LINE__ << ", m_own: " << tmp.m_own << "\n";
            std::cerr << ", id " << tmp.m_id  << "\n";
            std::cerr << ", ore" << tmp.m_num_of_ore  << "\n";
            std::cerr << ", facto" << tmp.num_factory  << "\n";
            std::cerr << ", mine" << tmp.num_mine  << "\n";
            std::cerr << ", fight" << tmp.num_fighters  << "\n";
            std::cerr << ", trans" << tmp.num_transporter  << "\n";
            std::cerr << ", attack" << m_attack_planet << "\n";


            if(m_attack_planet == 1)
            {
                tmp.m_attack_planet = true;
            }else
            {
                tmp.m_attack_planet = false;
            }
            if(socket_1 == socket)
            {
                pchanges_data1.push_back(tmp);
            }
            if(socket_2 == socket)
            {
                pchanges_data2.push_back(tmp);
            }
            //std::cout << pchanges_data1[0].m_own << ","<< pchanges_data1[0].m_id << "," << pchanges_data1[0].m_num_of_ore <<"," << tmp.num_factory <<"," << tmp.num_mine << "," <<tmp.num_fighters << ","   <<tmp.num_transporter << "," <<tmp.m_attack_planet << "\n";
        }
        if(socket_1 == socket)
        {
            std::cerr << __LINE__ << "\n";
            pchanges_size1 = size;
            m_socket_1_pchange_received = true;
        }
        if(socket_2 == socket)
        {
            std::cerr << __LINE__ << "\n";
            pchanges_size2 = size;
            m_socket_2_pchange_received = true;
        }
    }
    if((m_socket_1_pchange_received) && (m_socket_2_pchange_received))
    {
        std::cerr << __LINE__ << "\n";
        //pchanges_size1 = -1;
        //pchanges_size2 = -1;
        update_planet_changes();
    }
    std::cerr << "\n";
}

void Server::sendUpdatedPlanetChanges()
{
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    QByteArray data;
    int size = pchanges_committ.size();
    std::cerr << __LINE__ << ", " <<pchanges_committ.size() << ", size of vector\n";
    data.append(PacketType::planet_changes2d);
    data.append((char*)&size, 4);
    for(unsigned int i= 0 ; i < pchanges_committ.size(); i++)
    {
      pchanges_data tmp = pchanges_committ[i];
      data.append(tmp.m_own);
      data.append((char*)&tmp.m_id, 4);
      data.append((char*)&tmp.m_num_of_ore, 4);
      data.append((char*)&tmp.num_factory, 4);
      data.append((char*)&tmp.num_mine, 4);
      data.append((char*)&tmp.num_fighters, 4);
      data.append((char*)&tmp.num_transporter, 4);
       std::cout << size << "," << tmp.num_transporter <<  "," << tmp.num_mine << "\n";
      if(tmp.m_attack_planet)
      {
          char d = 1;
          data.append(d);
      }
      else
      {
          char d = 0;
          data.append(d);
      }
      send_changes = true;
    }
    if(socket_1->state() == QAbstractSocket::ConnectedState)
    {
        std::cerr << __LINE__ << ", writing something into socket";
        socket_1->write(data); //write the data itself
        socket_1->waitForBytesWritten();
    }
    if(socket_2->state() == QAbstractSocket::ConnectedState)
    {
        std::cerr << __LINE__ << ", writing something into socket";
        socket_2->write(data); //write the data itself
        socket_2->waitForBytesWritten();
    }
    pchanges_committ.clear();
    pchanges_data1.clear();
    pchanges_data2.clear();
    std::cerr << "\n";
}


void Server::update_planet_changes()
{
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << " updating changes\n";
    std::cerr << __LINE__ << ", pachnges.commit.siz() " << pchanges_committ.size() << "\n";
    // player1 attack
    for(auto it =pchanges_data1.begin() ; it!=pchanges_data1.end();)
    {
        if((*it).m_attack_planet == true)
        {
            //std::cout<< "planet attacking = true\n";
            std::cerr << __LINE__ << "\n";
            player1_outstanding_fights.push_back((*it));
            int id = it->m_id;
            it = pchanges_data1.erase(it);
            for(auto it2 =pchanges_data2.begin() ; it2!=pchanges_data2.end();)
            {
                if(it2->m_id == id)
                {
                    player2_outstanding_fights.push_back((*it2));
                    it2 = pchanges_data2.erase(it2);
                }  else
                {
                    it2++;
                }

            }
        } else {
            it++;
        }
    }

    std::cerr << __LINE__ << "\n";
    // player2 attack
    for(auto it = pchanges_data2.begin() ; it!=pchanges_data2.end(); )
    {
        if((*it).m_attack_planet)
        {
            std::cerr << __LINE__ << "\n";
            player2_outstanding_fights.push_back((*it));
            int id = it->m_id;
            it = pchanges_data2.erase(it);
            for(auto it1 =pchanges_data1.begin() ; it1!=pchanges_data1.end();)
            {
                if(it1->m_id == id)
                {
                    player1_outstanding_fights.push_back(*it1);
                    it1 = pchanges_data1.erase(it1);
                } else
                {
                    it1++;
                }

            }
        } else {
            it++;
        }
    }

    std::cerr << __LINE__ << ", pchanges_data1.size: " << pchanges_data1.size() << "\n";
    std::cerr << __LINE__ << ", pchanges_data2.size: " << pchanges_data2.size() << "\n";
    std::cerr << __LINE__ << "\n";
    // player2 and player1 want same planet
    for(auto it2 = pchanges_data2.begin() ; it2!=pchanges_data2.end(); )
    {
        std::cerr << __LINE__ << "\n";
        bool increment = true;
        for(auto it1 =pchanges_data1.begin() ; it1!=pchanges_data1.end();)
        {
            if(it2->m_id == it1->m_id)
            {
                std::cerr << __LINE__ << " \n";
                player1_outstanding_fights.push_back((*it1));
                it1 = pchanges_data1.erase(it1);
                player2_outstanding_fights.push_back((*it2));
                it2 = pchanges_data2.erase(it2);
                increment = false;
                break;
            } else {
                std::cerr << __LINE__ << " \n";
                it1++;
            }
        }
        if(increment)
        {
            it2++;
        }
    }

    std::cerr << __LINE__ << ", pchanges_data1.size: " << pchanges_data1.size() << "\n";
    std::cerr << __LINE__ << ", pchanges_data2.size: " << pchanges_data2.size() << "\n";
    for(auto it = pchanges_data1.begin(); it != pchanges_data1.end() ; it++)
    {
        std::cerr << __LINE__ << "\n";
        pchanges_committ.push_back((*it));
    }

    for(auto it = pchanges_data2.begin(); it != pchanges_data2.end() ; it++)
    {
        std::cerr << __LINE__<< "\n";
        pchanges_committ.push_back((*it));
    }

    if(player1_outstanding_fights.empty())
    {
        std::cerr << __LINE__ << "\n";
        sendUpdatedPlanetChanges();
    }
    else
    {
        std::cerr << __LINE__ << ", ";
        std::cerr << "player_outstanding_fights, " << player1_outstanding_fights.size() << "\n";
        std::cerr << "player_outstanding_fights, " << player2_outstanding_fights.size() << "\n";
        toSend = PacketType::init_3D;
        this->writeData(nullptr);
    }
    std::cerr << __LINE__ << ", pachnges.commit.siz() " << pchanges_committ.size() << "\n";
    std::cerr << "\n";
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
