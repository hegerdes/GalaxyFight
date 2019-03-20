#include "client.h"

namespace asteroids {
    Client::Client(QHostAddress addr, quint16 port)
    {
        socket.connectToHost(addr, port);
        std::cerr << global_socket.waitForConnected() << ": global_socket.waitForConnected\n";
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
        //
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
        if(global_socket.state() == QAbstractSocket::ConnectedState)
        {
            // wenn socket verbunden -> sende deine eigenen daten
            std::cout << "write data\n";

            global_socket.write(IntToArray(data.size())); //write size of data
            global_socket.write(data);
        }

    }

    void Client::readData(){
        // @ahaker
        if(global_socket.state() == QAbstractSocket::ConnectedState)
        {
            // empfange die positionen des anderen
            QByteArray answer = global_socket.readAll();
            if(answer.length() > 0){
                std::cerr << global_socket.waitForBytesWritten() << "; waitForBytesWritten\n";

                char* data = (char*) answer.data();

                PacketType pt = (PacketType) getChar(&data);
                if(pt == PacketType::init_3D){
                    //Own

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

                } else if(pt == PacketType::update_3D_S){
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

                    short count_astr = getShort(&data);

                    asteroids_deleted.clear();
                    asteroids_deleted.reserve(count_astr);
                    for(int i {0}; i < count_astr; i++){
                        asteroids_deleted.push_back(getInt(&data));
                    }

                    enemy_shot = (Bullet_shot) getChar(&data);
                    enemy_shot_id = getInt(&data);

                    own_hit = (Hit) getChar(&data);

                    int count_bullet = getInt(&data);

                    bullet_deleted.clear();
                    bullet_deleted.reserve(count_bullet);
                    for(int i {0}; i < count_bullet; i++){
                        bullet_deleted.push_back(getInt(&data));
                    }
                } else if(pt == PacketType::end_3D){
                    winner_no = getChar(&data);
                } else if(pt == PacketType::start_2D){
                    //TODO BLOCK WAITING:::
                }

            }
        }
    }

    float Client::getFloat(char** ptr){
        float f = **ptr;
        ptr += 4;
        return f;
    }

    short Client::getShort(char** ptr){
        short f = **ptr;
        ptr += 2;
        return f;
    }

    char Client::getChar(char** ptr){
        char f = **ptr;
        *ptr += 1;
        return f;
    }

    int Client::getInt(char** ptr){
        int f = **ptr;
        ptr += 4;
        return f;
    }
}
