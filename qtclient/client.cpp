#include "client.h"

namespace asteroids {
/**
 * @brief Client::Client the client connects to the server and manages the packets that arrive or are sent to the server
 */
Client::Client() {
    //set to true if the enemy disconnects, causes the game to stop
    enemy_disconnected = false;
}

/**
 * @brief Client::connect connects the client to the server
 * @param addr adress that the client connects to
 * @param port port that the server listens to
 */
void Client::connect(QString addr, quint16 port) {
    //socket.connectToHost(QHostAddress::LocalHost, 38292);
    socket.connectToHost(addr, port);
    //std::cerr << socket.waitForConnected() << ": socket.waitForConnected\n";
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
    //std::cerr << __LINE__ << ", " <<  __PRETTY_FUNCTION__ << ", " << size << ", size of recived package\n";
    std::list<PlanetChanges> p_changes;

    //RESET HEALTH::::

    own_health = 10;
    enemy_health = 10;

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
        /*std::cerr << "\t" << __LINE__ << __FUNCTION__ << ", m_own: " << m_own << ", m_id:" << m_id
                  << ", m_num_ore: " << m_num_of_ore
                  << ", num_factory: " << num_factory
                  << ", num_mine: " << num_mine
                  << ", num_fighters: " << num_fighters
                  << ", num_transporter: " << num_transporter
                  << ", attack_planet: " << m_attack_planet
                  << ", stored_ore: " << stored_ore << "\n";
                  */
    }
    p_changes_current = p_changes;
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
        int m_num_of_ore = (*it).getOre() * -1;
        int num_factory = (*it).getFactorys();
        int num_mine = (*it).getMines();
        int num_fighters = (*it).getFighter();
        int num_transporter = (*it).getTransports();
        bool m_attack_planet =(*it).getInitFight();
        int stored_ore = (*it).getOre() * -1;
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

/**
 * @brief Client::sendUpdate_3D_C update Package that the client sends to the Server so that the other client can
 * update its enemy SpaceCraft location
 * @param pos own position of fighter
 * @param xAxis axis of fighter
 * @param yAxis axis of fighter
 * @param zAxis axis of fighter
 * @param shot if enemy shot
 * @param living if player is living
 * @param bullet_id bullet that got shot
 */
void Client::sendUpdate_3D_C(Vector<float> pos, Vector<float> xAxis, Vector<float> yAxis, Vector<float> zAxis,
                             Bullet_shot shot, Living living, int bullet_id) {

    //std::cerr << "\nSEND BULLET ID CLIENT: " << bullet_id  << "\n";
    //std::cerr << "and shot type: " << shot << "\n";

    QByteArray data;
    data.append(PacketType::update_3D_C);
    data.append((char*) &pos, 3 * 4);
    data.append((char*) &xAxis, 3 * 4);
    data.append((char*) &yAxis, 3 * 4);
    data.append((char*) &zAxis, 3 * 4);
    data.append(shot);
    data.append((char*) &bullet_id, 4);
    // if shot save in list
    data.append(living);

    //write the data into buffer
    writeData(data);
}

/**
 * @brief Client::writeData writes the data that was given to the server-socket
 * @param data daata to be transmitted
 */
void Client::writeData(QByteArray const& data) {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        // wenn socket verbunden -> sende deine eigenen daten
        // //std::cerr << "write data\n";

        socket.write(IntToArray(data.size())); // write size of data
        socket.write(data);
    }
}
/**
 * @brief Client::init_3d packet that is send in 3D mode  to update enemy positions
 * @param data data array that will be manipulated and sent
 */
void Client::init_3d(char* data) {

    if(already_in_3d){
        return;
    }
    already_in_3d = true;

    std::cerr << "INIT PACKET RECEIVED\n";

    // own Position
    ownPos[0] = getFloat(&data);
    ownPos[1] = getFloat(&data);
    ownPos[2] = getFloat(&data);
    //std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\t" << ownPos << "\n";

    //own x-Axis
    ownxAxis[0] = getFloat(&data);
    ownxAxis[1] = getFloat(&data);
    ownxAxis[2] = getFloat(&data);

    //own y-Axis
    ownyAxis[0] = getFloat(&data);
    ownyAxis[1] = getFloat(&data);
    ownyAxis[2] = getFloat(&data);

    //own z-Axis
    ownzAxis[0] = getFloat(&data);
    ownzAxis[1] = getFloat(&data);
    ownzAxis[2] = getFloat(&data);

    // Enemy position
    enemyPos[0] = getFloat(&data);
    enemyPos[1] = getFloat(&data);
    enemyPos[2] = getFloat(&data);

    // enemy x-Axis
    enemyxAxis[0] = getFloat(&data);
    enemyxAxis[1] = getFloat(&data);
    enemyxAxis[2] = getFloat(&data);

    // enemy y-Axis
    enemyyAxis[0] = getFloat(&data);
    enemyyAxis[1] = getFloat(&data);
    enemyyAxis[2] = getFloat(&data);

    // enemy z-Axis
    enemyzAxis[0] = getFloat(&data);
    enemyzAxis[1] = getFloat(&data);
    enemyzAxis[2] = getFloat(&data);

    // amount of asteroids recived by Server
    count_astr = getInt(&data);

    //get position direction and size of asteroids
    for (int i{0}; i < count_astr; i++) {
        id_astr[i] = getInt(&data);

        pos_astr[i][0] = getFloat(&data);
        pos_astr[i][1] = getFloat(&data);
        pos_astr[i][2] = getFloat(&data);
        ////std::cerr << "Astr Pos: " << pos_astr[i] << "\n";

        dir_astr[i][0] = getFloat(&data);
        dir_astr[i][1] = getFloat(&data);
        dir_astr[i][2] = getFloat(&data);
        ////std::cerr << "Astr dir: " << dir_astr[i] << "\n";

        size_astr[i] = getFloat(&data);
    }

    // TODO:: TESTING___________________________________________
    player_No = (player_no) getChar(&data);
}

/**
 * @brief Client::update_3D_S update packet recived by Server to update enemy fighter data
 * @param data array to be manipulated
 */
void Client::update_3D_S(char* data) {

    //enemy fighter position
    enemyPos[0] = getFloat(&data);
    enemyPos[1] = getFloat(&data);
    enemyPos[2] = getFloat(&data);

    //enemy fighter x-Axis
    enemyxAxis[0] = getFloat(&data);
    enemyxAxis[1] = getFloat(&data);
    enemyxAxis[2] = getFloat(&data);

    //enemy fighter y-Axis
    enemyyAxis[0] = getFloat(&data);
    enemyyAxis[1] = getFloat(&data);
    enemyyAxis[2] = getFloat(&data);

    //enemy fighter z-Axis
    enemyzAxis[0] = getFloat(&data);
    enemyzAxis[1] = getFloat(&data);
    enemyzAxis[2] = getFloat(&data);

    // amount of asteroids
    short count_astr = getShort(&data);

    // put asteroids into list
    //asteroids_deleted.clear();

    // asteroids_deleted.reserve(count_astr);
    for (int i{0}; i < count_astr; i++) {
        asteroids_deleted.push_back(getInt(&data));
    }

    //read if enemy shot
    enemy_shot = (Bullet_shot) getChar(&data);

    //read bullet id
    enemy_shot_id = getInt(&data);


    own_hit = (Hit) getChar(&data);

    //amount of bullets that got shot
    int count_bullet = getInt(&data);

    //bullet_deleted.clear();
    // bullet_deleted.reserve(count_bullet);
    for (int i{0}; i < count_bullet; i++) {
        int in = getInt(&data);
        bullet_deleted.push_back(in);
        //std::cerr << "Bullet zum Löschen angekommen. ID: " << in << "\n";
    }

    //get health

    own_health = getInt(&data);
    enemy_health = getInt(&data);
    //std::cerr << "Enemy health " << enemy_health << " Own_Health: " << own_health << "\n";


    int num_bull_ids = getInt(&data);
    if(num_bull_ids != 0)
    {
        bullet_ids.clear();
        bullet_pos.clear();
        bullet_dirs.clear();
        updated_pos = true;
    }
    for(int i = num_bull_ids; i > 0; --i)
    {

        bullet_ids.push_back(getInt(&data));
        //adding and constructing new Vector3f to updated direction vector.
        Vector3f temp;
        temp[0] = getFloat(&data);
        temp[1] = getFloat(&data);
        temp[2] = getFloat(&data);
        bullet_pos.push_back(temp);
        //adding and constructing new Vector3f to updated direction vector.
        temp[0] = getFloat(&data);
        temp[1] = getFloat(&data);
        temp[2] = getFloat(&data);
        bullet_dirs.push_back(temp);

    }

    int num_aster_ids = getInt(&data);
    if(num_aster_ids != 0)
    {
        asteroid_ids.clear();
        asteroid_pos.clear();
        updated_pos = true;
    }
    for(int i = num_aster_ids; i > 0; i--)
    {
        asteroid_ids.push_back(getInt(&data));
        //adding and constructing new Vector3f to updated
        Vector3f temp;
        temp[0] = getFloat(&data);
        temp[1] = getFloat(&data);
        temp[2] = getFloat(&data);
        asteroid_pos.push_back(temp);
    }


}
/**
 * @brief Client::readData interpretes the answer after it got recived by the server
 */
void Client::readData() {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        socket.waitForReadyRead();
        // interprets the package
        interpreteAnswer();
    }
}

/**
 * @brief Client::sendReadyT send the Ready package with id of the player when connected
 * @param player_id id of the player
 * @param length length of the id
 */
void Client::sendReadyT(char* player_id, int length)
{
    QByteArray data;
    data.append(PacketType::ready_T);
    data.append((char*)&length, 4);
    data.append(player_id, length);
    writeData(data);
}

/**
 * @brief Client::conLost true if enemy lost the connection
 */
void Client::conLost()
{
    enemy_disconnected = true;
    //std::cerr << "Lost connection to server!\n";
}

/**
 * @brief Client::game_start package recived by the server when both players have connected
 * @param data
 */
void Client::game_start(char* data)
{   // length of the id
    int length = getInt(&data);

    //read the id
    char id[length + 1];
    for(int i = 0; i < length; i++)
    {
        id[i] = getChar(&data);
    }
    id[length] = '\0';
    id_other = std::string(id);

    //set the player-id
    player_No = (player_no) getChar(&data);

    //MapKonfig erste mal laden.
}

/**
 * @brief Client::interpreteAnswer interprets the package recived and gives an answer to the server
 */
void Client::interpreteAnswer() {
    //std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    QByteArray answer = socket.readAll();
    if (answer.length() > 0) {

        // //std::cerr << socket.waitForBytesWritten() << "; waitForBytesWritten\n";
        //std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
        socket.waitForBytesWritten();
        char* data = (char*) answer.data();
        //std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";

        //interpret the packettype that got recived
        PacketType pt = (PacketType) getChar(&data);
        if (pt == PacketType::init_3D) {
            // Own
            //std::cerr << "\t" << __LINE__ << __FUNCTION__ << "init_3d\n";
            init_3d(data);
            init_received = true;
        } else if (pt == PacketType::update_3D_S) {
            ////std::cerr << __LINE__ << "\n";
            // //std::cerr << "packtetype_udpate_3d_s\n";
            update_3D_S(data);
        } else if(pt == PacketType::planet_changes2d) {
                //std::cerr << "\t" << __LINE__ << __FUNCTION__ << ", planet changes got recived\n";
                already_in_3d = false;
                std::cerr << "RESET:::::: OF INIT PACKET RECEIVED\n";
                recivePlanetChanges(data);
                m_planet_changes_received = true;
        } else if (pt == PacketType::end_3D) {
            //std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
            winner_no = getChar(&data);

        } else if (pt == PacketType::start_2D) {
            //std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
            // TODO BLOCK WAITING:::
        } else if(pt == PacketType::game_start)
        {
            //std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
            game_start(data);
        } else {
            //std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";

        }
            //std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
    }
    ////std::cerr << __LINE__ << ", end_________" << __PRETTY_FUNCTION__ << "\n";
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

/**
 * @brief Client::wait_for_readData waits for the data to be transmitted
 * @param timeout timeout amount
 */
bool Client::wait_for_readData(int timeout) {
    bool read_status = false;
    if (socket.state() == QAbstractSocket::ConnectedState) {
        // empfange die positionen des anderen
        read_status = socket.waitForReadyRead(timeout);
        interpreteAnswer();
    }
    return read_status;
}
/**
 * @brief Client::getFloat maniupulates the pointer and return float
 * @param ptr pointer to be manipulated
 * @return  float
 */
float Client::getFloat(char** ptr) {
    float* jo = (float*) *ptr;
    float f = *jo;
    *ptr += 4;
    return f;
}
/**
 * @brief Client::getSHort maniupulates the pointer and return float
 * @param ptr pointer to be manipulated
 * @return  short value at position of pointer
 */
short Client::getShort(char** ptr) {
    short* jo = (short*) *ptr;
    short f = *jo;
    *ptr += 2;
    return f;
}
/**
 * @brief Client::getChar maniupulates the pointer and return float
 * @param ptr pointer to be manipulated
 * @return  char value at position of pointer
 */
char Client::getChar(char** ptr) {
    char f = **ptr;
    *ptr += 1;
    return f;
}
/**
 * @brief Client::getInt maniupulates the pointer and return float
 * @param ptr pointer to be manipulated
 * @return  int value at the position of pointer
 */
int Client::getInt(char** ptr) {
    int* jo = (int*) *ptr;
    int f = *jo;
    *ptr += 4;
    return f;
}
} // namespace asteroids
