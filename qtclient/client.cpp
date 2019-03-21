#include "client.h"

namespace asteroids {
    Client::Client()
    {}

    void Client::connect(QString addr, quint16 port)
    {
        //socket.connectToHost(QHostAddress::LocalHost, port);
        socket.connectToHost(addr, port);
        std::cerr << socket.waitForConnected() << ": socket.waitForConnected\n";
    }

    QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
    {
        //Avoid use of cast, this is the Qt way to serialize objects
        QByteArray temp;
        QDataStream data(&temp, QIODevice::ReadWrite);
        data << source;
        return temp;
    }

    void Client::sendUpdate_3D_C(Vector<float> pos, Vector<float> xAxis, Vector<float> yAxis, Vector<float> zAxis, Bullet_shot shot, Living living, int bullet_id){
        QByteArray data;
        data.append(PacketType::update_3D_C);
        data.append((char*)&pos, 3*4);
        data.append((char*)&xAxis, 3*4);
        data.append((char*)&yAxis, 3*4);
        data.append((char*)&zAxis, 3*4);
        data.append(shot);
        data.append((char*)&bullet_id, 4);

        //TODO if shot save in list
        data.append(living);

        writeData(data);
    }

    void Client::writeData(QByteArray const &data){
        if(socket.state() == QAbstractSocket::ConnectedState)
        {
            // wenn socket verbunden -> sende deine eigenen daten
            //std::cerr << "write data\n";

            socket.write(IntToArray(data.size())); //write size of data
            socket.write(data);
        }

    }

    void Client::init_3d(char * data){
        ownPos[0] = getFloat(&data);
        ownPos[1] = getFloat(&data);
        ownPos[2] = getFloat(&data);

        ownxAxis[0] = getFloat(&data);
        ownxAxis[1] = getFloat(&data);
        ownxAxis[2] = getFloat(&data);

        ownyAxis[0] = getFloat(&data);
        ownyAxis[1] = getFloat(&data);
        ownyAxis[2] = getFloat(&data);
        ownzAxis[0] = getFloat(&data);
        ownzAxis[1] = getFloat(&data);
        ownzAxis[2] = getFloat(&data);

        //Enemy

        enemyPos[0] = getFloat(&data);
        enemyPos[1] = getFloat(&data);
        enemyPos[2] = getFloat(&data);

        enemyxAxis[0] = getFloat(&data);
        enemyxAxis[1] = getFloat(&data);
        enemyxAxis[2] = getFloat(&data);

        enemyyAxis[0] = getFloat(&data);
        enemyyAxis[1] = getFloat(&data);
        enemyyAxis[2] = getFloat(&data);

        enemyzAxis[0] = getFloat(&data);
        enemyzAxis[1] = getFloat(&data);
        enemyzAxis[2] = getFloat(&data);

        int count_astr = getInt(&data);
        for(int i {0}; i < count_astr; i++){
            //TODO creating asteroids
        }

    }

    void Client::update_3D_S(char* data)
    {
        enemyPos[0] = getFloat(&data);
        enemyPos[1] = getFloat(&data);
        enemyPos[2] = getFloat(&data);
        if(enemyPos[0] != -650 || enemyPos[1] || enemyPos[2] ) { std::cerr <<"enemy_pos: " << enemyPos[0] << "," << enemyPos[1] << "," << enemyPos[2] << "\n"; }

        enemyxAxis[0] = getFloat(&data);
        enemyxAxis[1] = getFloat(&data);
        enemyxAxis[2] = getFloat(&data);
        if(enemyxAxis[0] != -1 || enemyxAxis[1] || enemyxAxis[2] ) { std::cerr << enemyxAxis[0] << "," << enemyxAxis[1] << "," << enemyxAxis[2] << "\n"; }

        enemyyAxis[0] = getFloat(&data);
        enemyyAxis[1] = getFloat(&data);
        enemyyAxis[2] = getFloat(&data);
        if(enemyyAxis[0] || enemyyAxis[1] != -1 || enemyyAxis[2] ) { std::cerr << enemyyAxis[0] << "," << enemyyAxis[1] << "," << enemyyAxis[2] << "\n"; }

        enemyzAxis[0] = getFloat(&data);
        enemyzAxis[1] = getFloat(&data);
        enemyzAxis[2] = getFloat(&data);
        if(enemyzAxis[0] || enemyzAxis[1] || enemyzAxis[2] != 1 ) { std::cerr << enemyzAxis[0] << "," << enemyzAxis[1] << "," << enemyzAxis[2] << "\n"; }

        short count_astr = getShort(&data);
        if(count_astr) { std::cerr << "Asteroid count: " << count_astr << "\n"; }
        asteroids_deleted.clear();
        //asteroids_deleted.reserve(count_astr);
        for(int i {0}; i < count_astr; i++){
            asteroids_deleted.push_back(getInt(&data));
        }

        enemy_shot = (Bullet_shot) getChar(&data);
        if(enemy_shot) { std::cerr << "Enemy shot: " << enemy_shot << "\n"; }

        enemy_shot_id = getInt(&data);
        if(enemy_shot_id) { std::cerr << "Enemy shot_id: " << enemy_shot_id << "\n"; }

        own_hit = (Hit) getChar(&data);
        if(own_hit) { std::cerr << "own_hit: " << own_hit << "\n"; }

        int count_bullet = getInt(&data);
        if(count_bullet) { std::cerr << "Bullet count: " << count_bullet << "\n\n"; }
        bullet_deleted.clear();
        //bullet_deleted.reserve(count_bullet);
        for(int i {0}; i < count_bullet; i++){
            bullet_deleted.push_back(getInt(&data));
        }
    }

    void Client::readData(){
        //std::cerr << "read data\n";
        if(socket.state() == QAbstractSocket::ConnectedState)
        {
            // empfange die positionen des anderen
            QByteArray answer = socket.readAll();
            if(answer.length() > 0){
                //std::cerr << socket.waitForBytesWritten() << "; waitForBytesWritten\n";
                socket.waitForBytesWritten();
                char* data = (char*) answer.data();

                PacketType pt = (PacketType) getChar(&data);
                //std::cerr << "pid: " << pt << ", length" << answer.length() << "\n";
                if(pt == PacketType::init_3D){
                    //Own
                    init_3d(data);
                } else if(pt == PacketType::update_3D_S){
                    //std::cerr << "packtetype_udpate_3d_s\n";
                    update_3D_S(data);
                } else if(pt == PacketType::end_3D){
                    winner_no = getChar(&data);
                } else if(pt == PacketType::start_2D){
                    //TODO BLOCK WAITING:::
                }


            }
        }
    }

    float Client::getFloat(char** ptr){
        float * jo = (float*)*ptr;
        float f = *jo;
        *ptr += 4;
        return f;
    }

    short Client::getShort(char** ptr){
        short * jo = (short*)*ptr;
        short f = *jo;
        *ptr += 2;
        return f;
    }

    char Client::getChar(char** ptr){
        char f = **ptr;
        *ptr += 1;
        return f;
    }

    int Client::getInt(char** ptr){
        int * jo = (int*)*ptr;
        int f = *jo;
        *ptr += 4;
        return f;
    }
}
