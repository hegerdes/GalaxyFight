#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include <iostream>
#include <math/Vector.hpp>
#include <qtserver/flags.h>
#include <qtserver/packettypes.h>
#include <vector>

namespace asteroids {
class Client {
  public:
    Client();

    // init_3D
    Vector<float> ownPos;
    Vector<float> ownxAxis;
    Vector<float> ownyAxis;
    Vector<float> ownzAxis;


    //game_start
    std::string id_other;
    char player_No;
    //Map Konfig am start fehlt

    Vector3f pos_astr[10];
    Vector3f dir_astr[10];
    float size_astr[10];
    int count_astr;
    int id_astr[10];

    // update_3D_S / init_3D
    Vector<float> enemyPos;
    Vector<float> enemyxAxis;
    Vector<float> enemyyAxis;
    Vector<float> enemyzAxis;
    Bullet_shot enemy_shot;
    int enemy_shot_id;

    std::vector<int> asteroids_deleted;
    std::vector<int> bullet_deleted;
    Hit own_hit;

    // end_3D
    char winner_no;

    void sendUpdate_3D_C(Vector<float> pos, Vector<float> xAxis, Vector<float> yAxis, Vector<float> zAxis,
                         Bullet_shot shot, Living living, int bullet_id);
    void readData();
    void wait_for_readData(int timeout);


    //start_2D
    /**
     * @brief sendReadyT constructs the ready_T packet. This will signal the server, the user is ready to play
     * @param player_id playername as c_str without terminating 0-Byte
     */
    void sendReadyT(char* player_id, int length);
    void game_start(char* data);
    void conLost();

    void connect(QString addr, quint16 port);
    bool init_received;

  private:
    void writeData(QByteArray const& data);
    void init_3d(char*);
    void update_3D_S(char*);

    QTcpSocket socket;
    float getFloat(char** ptr);
    int getInt(char** ptr);
    char getChar(char** ptr);
    short getShort(char** ptr);
    void interpreteAnswer();
};
} // namespace asteroids

#endif // CLIENT_H
