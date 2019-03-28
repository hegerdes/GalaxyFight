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
    QByteArray data;
    data.append(PacketType::update_3D_C);
    data.append((char*) &pos, 3 * 4);
    data.append((char*) &xAxis, 3 * 4);
    data.append((char*) &yAxis, 3 * 4);
    data.append((char*) &zAxis, 3 * 4);
    data.append(shot);
    data.append((char*) &bullet_id, 4);
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
        // std::cerr << "write data\n";

        socket.write(IntToArray(data.size())); // write size of data
        socket.write(data);
    }
}
/**
 * @brief Client::init_3d packet that is send in 3D mode  to update enemy positions
 * @param data data array that will be manipulated and sent
 */
void Client::init_3d(char* data) {

    // own Position
    ownPos[0] = getFloat(&data);
    ownPos[1] = getFloat(&data);
    ownPos[2] = getFloat(&data);
    std::cerr << ownPos << "\n";

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
    std::cerr << count_astr << " count_astr --------------------\n";

    //get position direction and size of asteroids
    for (int i{0}; i < count_astr; i++) {
        id_astr[i] = getInt(&data);

        pos_astr[i][0] = getFloat(&data);
        pos_astr[i][1] = getFloat(&data);
        pos_astr[i][2] = getFloat(&data);
        std::cerr << "Astr Pos: " << pos_astr[i] << "\n";

        dir_astr[i][0] = getFloat(&data);
        dir_astr[i][1] = getFloat(&data);
        dir_astr[i][2] = getFloat(&data);
        std::cerr << "Astr dir: " << dir_astr[i] << "\n";

        size_astr[i] = getFloat(&data);
    }
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
    asteroids_deleted.clear();
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

    bullet_deleted.clear();
    // bullet_deleted.reserve(count_bullet);
    for (int i{0}; i < count_bullet; i++) {
        bullet_deleted.push_back(getInt(&data));
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
    std::cerr << __LINE__ << ", " << player_No << " player_No";

    //MapKonfig erste mal laden.
}

/**
 * @brief Client::interpreteAnswer interprets the package recived and gives an answer to the server
 */
void Client::interpreteAnswer() {

    // read everything from socket
    QByteArray answer = socket.readAll();
    if (answer.length() > 0) {

        socket.waitForBytesWritten();
        char* data = (char*) answer.data();

        //interpret the packettype that got recived
        PacketType pt = (PacketType) getChar(&data);

        if (pt == PacketType::init_3D) {
            // Own
            init_3d(data);
            init_received = true;
        } else if (pt == PacketType::update_3D_S) {
            // std::cerr << "packtetype_udpate_3d_s\n";
            update_3D_S(data);
        } else if (pt == PacketType::end_3D) {
            winner_no = getChar(&data);
        } else if (pt == PacketType::start_2D) {
            // TODO BLOCK WAITING:::
        } else if(pt == PacketType::game_start)
        {
            game_start(data);
        }
    }
}
/**
 * @brief Client::wait_for_readData waits for the data to be transmitted
 * @param timeout timeout amount
 */
void Client::wait_for_readData(int timeout) {
    // std::cerr << "read data\n";
    if (socket.state() == QAbstractSocket::ConnectedState) {
        // empfange die positionen des anderen
        socket.waitForReadyRead(timeout);
        interpreteAnswer();
    }
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
