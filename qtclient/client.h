#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include <iostream>
#include <math/Vector.hpp>
#include <qtserver/flags.h>
#include <qtserver/packettypes.h>
#include <vector>
#include "../rendering/2D/PlanetChanges.hpp"

namespace asteroids {
class Client {
  public:
    Client();

    // init_3D
    Vector<float> ownPos;
    Vector<float> ownxAxis;
    Vector<float> ownyAxis;
    Vector<float> ownzAxis;


    //con_lost
    bool enemy_disconnected;

    //game_start
    std::string id_other;
    player_no player_No;
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

    /**
     * @brief sendUpdate_3D_C this method should be called to send a new update to he server. It should be called at least in every iteration of of the mainloop
     * @param pos Vector<float> the current position of the own spacecraft
     * @param xAxis Vector<float> the current X-Axis of the spacecraft
     * @param yAxis Vector<float> the current Y-Axis of the spacecraft
     * @param zAxis Vector<float> the current Z-Axis of the spacecraft
     * @param shot Bulllet_shot signals if a bullet was shot iin this iteration
     * @param living Living signals if the spacecraft has more than 1 HP
     * @param bullet_id int the current id of the shot bullet
     */
    void sendUpdate_3D_C(Vector<float> pos, Vector<float> xAxis, Vector<float> yAxis, Vector<float> zAxis,
                         Bullet_shot shot, Living living, int bullet_id);
    void readData();
    bool wait_for_readData(int timeout);
    void SendPlanetChanges(int size,std::list<PlanetChanges> changes );
    void recivePlanetChanges(char * data);


    //start_2D
    /**
     * @brief sendReadyT constructs the ready_T packet. This will signal the server, the user is ready to play
     * @param player_id playername as c_str without terminating 0-Byte
     */
    void sendReadyT(char* player_id, int length);
    /**
     * @brief game_start interprets the game_start packet and updates the relevant vars
     * @param data char* the received data without the id_Byte infront
     */
    void game_start(char* data);
    /**
     * @brief conLost sets the enemy_disconnected flag to true. It is called whenever the con_lost packet is received. Only has an effect, if the method stillPlayable() is called frequently
     */
    void conLost();

    /**
     * @brief stillPlayable returns false if the enemy player was disconnected unexpectedly
     */
    bool stillPlayable()
    {
        return !enemy_disconnected;
    }

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
