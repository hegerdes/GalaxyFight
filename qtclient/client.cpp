#include "client.h"

namespace asteroids {
Client::Client() {
    enemy_disconnected = false;
}

void Client::connect(QString addr, quint16 port) {
    socket.connectToHost(QHostAddress::LocalHost, 38292);
    //socket.connectToHost(addr, port);
    std::cerr << socket.waitForConnected() << ": socket.waitForConnected\n";
}

QByteArray IntToArray(qint32 source) // Use qint32 to ensure that the number have 4 bytes
{
    // Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}


void Client::recivePlanetChanges(char * data)
{
    int size = getInt(&data);
    std::cerr << __LINE__ << ", " << size << ", size of recived package\n";
    std::list<PlanetChanges> p_changes;
    for (int i {0} ; i < size ; i++)
    {
        PlanetChanges::Owner m_own  = (PlanetChanges::Owner)getChar(&data);
        int m_id = getInt(&data);
        int m_num_of_ore = getInt(&data);
        int num_factory = getInt(&data);
        int num_mine = getInt(&data);
        int num_fighters = getInt(&data);
        int num_transporter = getInt(&data);
        bool m_attack_planet = (bool)getChar(&data);
        int stored_ore = getInt(&data);
        bool m_attack;
        if(m_attack_planet == 1)
        {
            m_attack = true;
        } else
        {
            m_attack = false;
        }
        p_changes.push_back(PlanetChanges(m_own, m_id, m_num_of_ore,
                                          num_factory, num_mine, num_fighters,
                                          num_transporter, m_attack, stored_ore));
        std::cerr << __LINE__ << ", m_own: " << m_own << ", m_id:" << m_id
                  << ", m_num_ore: " << m_num_of_ore
                  << ", num_factory: " << num_factory
                  << ", num_mine: " << num_mine
                  << ", num_fighters: " << num_fighters
                  << ", num_transporter: " << num_transporter
                  << ", attack_planet: " << m_attack_planet
                  << ", stored_ore: " << stored_ore << "\n";
    }
}


void Client::SendPlanetChanges(int size,std::list<PlanetChanges> changes )
{
    QByteArray data;
    // packettype
    data.append(PacketType::planet_changes2d);

    // size of list
    data.append((char*)&size, 4);
    // start iterating through every element of the list
    for(auto it =changes.begin() ; it!= changes.end(); it++)
    {
        asteroids::PlanetChanges::Owner owner =(*it).getOwner();
        int m_id = (*it).getID();
        int m_num_of_ore = (*it).getOre();
        int num_factory = (*it).getFactorys();
        int num_mine = (*it).getMines();
        int num_fighters = (*it).getFighter();
        int num_transporter = (*it).getTransports();
        bool m_attack_planet =(*it).getInitFight();
        int stored_ore = (*it).getOre();
        char m_attack = 0;
        if(m_attack_planet == true)
        {
            m_attack = 1;
        } else
        {
            m_attack = 0;
        }
        data.append(owner);
        data.append((char*)&m_id, sizeof(m_id));
        data.append((char*)&m_num_of_ore, 4);
        data.append((char*)&num_factory, 4);
        data.append((char*)&num_mine, 4);
        data.append((char*)&num_fighters, 4);
        data.append((char*)&num_transporter, 4);
        data.append((char*)&m_attack,1);
        data.append((char*)&stored_ore, 4);

    }
    writeData(data);
}

void Client::sendUpdate_3D_C(Vector<float> pos, Vector<float> xAxis, Vector<float> yAxis, Vector<float> zAxis,
                             Bullet_shot shot, Living living, int bullet_id) {
    QByteArray data;
    data.append(PacketType::update_3D_C);
    data.append((char*) &pos, 3 * 4);
    data.append((char*) &xAxis, 3 * 4);
    data.append((char*) &yAxis, 3 * 4);
    data.append((char*) &zAxis, 3 * 4);
    data.append(shot);
    data.append((char*) &bullet_id, 4);
    // TODO if shot save in list
    data.append(living);

    writeData(data);
}

void Client::writeData(QByteArray const& data) {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        // wenn socket verbunden -> sende deine eigenen daten
        // std::cerr << "write data\n";

        socket.write(IntToArray(data.size())); // write size of data
        socket.write(data);
    }
}

void Client::init_3d(char* data) {
    ownPos[0] = getFloat(&data);
    ownPos[1] = getFloat(&data);
    ownPos[2] = getFloat(&data);
    std::cerr << ownPos << "\n";

    ownxAxis[0] = getFloat(&data);
    ownxAxis[1] = getFloat(&data);
    ownxAxis[2] = getFloat(&data);

    ownyAxis[0] = getFloat(&data);
    ownyAxis[1] = getFloat(&data);
    ownyAxis[2] = getFloat(&data);
    ownzAxis[0] = getFloat(&data);
    ownzAxis[1] = getFloat(&data);
    ownzAxis[2] = getFloat(&data);

    // Enemy

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

    count_astr = getInt(&data);
    //std::cerr << count_astr << " count_astr --------------------\n";
    for (int i{0}; i < count_astr; i++) {
        id_astr[i] = getInt(&data);

        pos_astr[i][0] = getFloat(&data);
        pos_astr[i][1] = getFloat(&data);
        pos_astr[i][2] = getFloat(&data);
        //std::cerr << "Astr Pos: " << pos_astr[i] << "\n";

        dir_astr[i][0] = getFloat(&data);
        dir_astr[i][1] = getFloat(&data);
        dir_astr[i][2] = getFloat(&data);
        //std::cerr << "Astr dir: " << dir_astr[i] << "\n";

        size_astr[i] = getFloat(&data);
    }


    // TODO:: TESTING___________________________________________
    player_No = (player_no) getChar(&data);
}

void Client::update_3D_S(char* data) {
    enemyPos[0] = getFloat(&data);
    enemyPos[1] = getFloat(&data);
    enemyPos[2] = getFloat(&data);
    // if(enemyPos[0] != -650 || enemyPos[1] || enemyPos[2] ) { std::cerr <<"enemy_pos: " << enemyPos[0] << "," <<
    // enemyPos[1] << "," << enemyPos[2] << "\n"; }

    enemyxAxis[0] = getFloat(&data);
    enemyxAxis[1] = getFloat(&data);
    enemyxAxis[2] = getFloat(&data);
    // if(enemyxAxis[0] != -1 || enemyxAxis[1] || enemyxAxis[2] ) { std::cerr << enemyxAxis[0] << "," << enemyxAxis[1]
    // << "," << enemyxAxis[2] << "\n"; }

    enemyyAxis[0] = getFloat(&data);
    enemyyAxis[1] = getFloat(&data);
    enemyyAxis[2] = getFloat(&data);
    // if(enemyyAxis[0] || enemyyAxis[1] != -1 || enemyyAxis[2] ) { std::cerr << enemyyAxis[0] << "," << enemyyAxis[1]
    // << "," << enemyyAxis[2] << "\n"; }

    enemyzAxis[0] = getFloat(&data);
    enemyzAxis[1] = getFloat(&data);
    enemyzAxis[2] = getFloat(&data);
    // if(enemyzAxis[0] || enemyzAxis[1] || enemyzAxis[2] != 1 ) { std::cerr << enemyzAxis[0] << "," << enemyzAxis[1] <<
    // "," << enemyzAxis[2] << "\n"; }

    short count_astr = getShort(&data);
    // if(count_astr) { std::cerr << "Asteroid count: " << count_astr << "\n"; }
    asteroids_deleted.clear();
    // asteroids_deleted.reserve(count_astr);
    for (int i{0}; i < count_astr; i++) {
        asteroids_deleted.push_back(getInt(&data));
    }

    enemy_shot = (Bullet_shot) getChar(&data);
    // if(enemy_shot) { std::cerr << "Enemy shot: " << enemy_shot << "\n"; }

    enemy_shot_id = getInt(&data);
    // if(enemy_shot_id) { std::cerr << "Enemy shot_id: " << enemy_shot_id << "\n"; }

    own_hit = (Hit) getChar(&data);
    // if(own_hit) { std::cerr << "own_hit: " << own_hit << "\n"; }

    int count_bullet = getInt(&data);
    // if(count_bullet) { std::cerr << "Bullet count: " << count_bullet << "\n\n"; }
    bullet_deleted.clear();
    // bullet_deleted.reserve(count_bullet);
    for (int i{0}; i < count_bullet; i++) {
        bullet_deleted.push_back(getInt(&data));
    }
}

void Client::readData() {
    // std::cerr << "read data\n";
    if (socket.state() == QAbstractSocket::ConnectedState) {
        // empfange die positionen des anderen
        socket.waitForReadyRead();
        interpreteAnswer();
    }
}


void Client::sendReadyT(char* player_id, int length)
{
    QByteArray data;
    data.append(PacketType::ready_T);
    data.append((char*)&length, 4);
    data.append(player_id, length);
    writeData(data);
}

void Client::conLost()
{
    enemy_disconnected = true;
}

void Client::game_start(char* data)
{
    int length = getInt(&data);
    char id[length + 1];
    for(int i = 0; i < length; i++)
    {
        id[i] = getChar(&data);
    }
    id[length] = '\0';
    id_other = std::string(id);
    player_No = (player_no) getChar(&data);

    //MapKonfig erste mal laden.
}

void Client::interpreteAnswer() {
    //std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    QByteArray answer = socket.readAll();
    if (answer.length() > 0) {
        // std::cerr << socket.waitForBytesWritten() << "; waitForBytesWritten\n";
        socket.waitForBytesWritten();
        char* data = (char*) answer.data();

        PacketType pt = (PacketType) getChar(&data);
        // std::cerr << "pid: " << pt << ", length" << answer.length() << "\n";
        if (pt == PacketType::init_3D) {
            // Own
            std::cerr << __LINE__ << "\n";
            init_3d(data);
            init_received = true;
        } else if (pt == PacketType::update_3D_S) {
            //std::cerr << __LINE__ << "\n";
            // std::cerr << "packtetype_udpate_3d_s\n";
            update_3D_S(data);
        } else if(pt == PacketType::planet_changes2d) {
                std::cerr << __LINE__  << ", planet changes got recived\n";
                recivePlanetChanges(data);
                m_planet_changes_received = true;
        } else if (pt == PacketType::end_3D) {
            std::cerr << __LINE__ << "\n";
            winner_no = getChar(&data);
        } else if (pt == PacketType::start_2D) {
            std::cerr << __LINE__ << "\n";
            // TODO BLOCK WAITING:::
        } else if(pt == PacketType::game_start)
        {
            std::cerr << __LINE__ << "\n";
            game_start(data);
        } else {
            std::cerr << __LINE__ << "\n";

        }
    }
    //std::cerr << __LINE__ << ", end_________" << __PRETTY_FUNCTION__ << "\n";
}

void Client::send_reset_planet_changes(){
    QByteArray data;
    data.append(PacketType::reset_planet_changes);
    writeData(data);
};

void Client::rerequest_planet_changes(){
    QByteArray data;
    data.append(PacketType::rerequest_planet_changes);
    writeData(data);
};

void Client::send_end_3d(player_no player_3d_winner){
    QByteArray data;
    data.append(PacketType::end_3D);
    data.append(player_3d_winner);
    data.append(player_No);
    writeData(data);
};

bool Client::wait_for_readData(int timeout) {
    // std::cerr << "read data\n";
    bool read_status = false;
    if (socket.state() == QAbstractSocket::ConnectedState) {
        // empfange die positionen des anderen
        read_status = socket.waitForReadyRead(timeout);
        interpreteAnswer();
    }
    return read_status;
}
float Client::getFloat(char** ptr) {
    float* jo = (float*) *ptr;
    float f = *jo;
    *ptr += 4;
    return f;
}

short Client::getShort(char** ptr) {
    short* jo = (short*) *ptr;
    short f = *jo;
    *ptr += 2;
    return f;
}

char Client::getChar(char** ptr) {
    char f = **ptr;
    *ptr += 1;
    return f;
}

int Client::getInt(char** ptr) {
    int* jo = (int*) *ptr;
    int f = *jo;
    *ptr += 4;
    return f;
}
} // namespace asteroids
