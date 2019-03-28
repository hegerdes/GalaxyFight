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

    /**
     * @brief ownPos Position of the SpaceCraft that will be transmitted to the Server
     */
    Vector<float> ownPos;
    /**
     * @brief ownxAxis Axis of the SpaceCraft
     */
    Vector<float> ownxAxis;
    /**
     * @brief ownyAxis Axis of the SpaceCraft
     */
    Vector<float> ownyAxis;
    /**
     * @brief ownzAxis Axis of the SpaceCraft
     */
    Vector<float> ownzAxis;

    /**
     * @brief enemyPos position of the enemy SpaceCraft
     */
    Vector<float> enemyPos;
    /**
     * @brief enemyxAxis of the SpaceCraft
     */
    Vector<float> enemyxAxis;
    /**
     * @brief enemyyAxis of the enemy SpaceCraft
     */
    Vector<float> enemyyAxis;
    /**
     * @brief enemyzAxis of the enemy SpaceCraft
     */
    Vector<float> enemyzAxis;
    /**
     * @brief enemy_shot enum on shot if enemy got shot
     */
    Bullet_shot enemy_shot;
    /**
     * @brief enemy_shot_id id of the bullet that hit the enemy
     */
    int enemy_shot_id;


    /**
     * @brief enemy_disconnected is set if the enemy disconnects via the con_los packet
     */
    bool enemy_disconnected;

    /**
     * @brief id_other id of the other player
     */
    std::string id_other;

    /**
     * @brief player_No player No sent by the server
     */
    player_no player_No = player_no::unassigned;
    //Map Konfig am start fehlt

    /**
     * @brief pos_astr will be recived by Init3D package
     */
    Vector3f pos_astr[10];
    /**
     * @brief dir_astr recived by Init3D package
     */
    Vector3f dir_astr[10];
    /**
     * @brief size_astr recived by Init3D package from server
     */
    float size_astr[10];
    /**
     * @brief count_astr amount of astroids
     */
    int count_astr;
    /**
     * @brief id_astr id of every asteroids
     */
    int id_astr[10];

    /**
     * @brief asteroids_deleted asteroids that have to be deleted
     */
    std::vector<int> asteroids_deleted;
    /**
     * @brief bullet_deleted bullets to be deleted
     */
    std::vector<int> bullet_deleted;

    /**
     * @brief own_hit if got hit
     */
    Hit own_hit;

    int own_health = 10;
    int enemy_health = 10;

    std::vector<int> bullet_ids;
    std::vector<Vector3f> bullet_pos;
    std::vector<Vector3f> bullet_dirs;
    std::vector<int> asteroid_ids;
    std::vector<Vector3f> asteroid_pos;
    bool updated_pos = false;

    // end_3D
    /**
     * @brief winner_no winner of 3D fight for end3d package
     */
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

    /**
     * @brief readData reads incoming data and interprets the package that got recived
     */
    void readData();
    /**
     * @brief wait_for_readData blocks until data got recived
     * @param timeout time in ms to wait for data that got read
     */
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
    /**
     * @brief connect connects the client to a server with add and port
     * @param addr adress you want to connecto to as string
     * @param port to be connected as int value
     */
    void connect(QString addr, quint16 port);

    /**
     * @brief init_received set to true if init packet got recived
     */
    bool init_received;
    void send_end_3d(player_no);
    bool m_planet_changes_received = false;
    int m_own_received_hp = 0;
    void send_reset_planet_changes();
    void rerequest_planet_changes();
    std::list<PlanetChanges> p_changes_current;

  private:
    /**
     * @brief socket to transmit the data to the server
     */
    QTcpSocket socket;
    /**
     * @brief writeData writes the data to the server via socket
     * @param data data to be sent
     */
    void writeData(QByteArray const& data);
    /**
     * @brief init_3d recives the init3D package and uses the information to initalize asteroids and SpaceCrafts
     */
    void init_3d(char*);


    /**
     * @brief update_3D_S recives the current position of the enemy SpaceCraft
     */
    void update_3D_S(char*);

    /**
     * @brief functions to manipulate a pointer depending on the value you want
     * @param ptr pointer to be manipulated
     * @return the value you want to have
     */
    float getFloat(char** ptr);
    int getInt(char** ptr);
    char getChar(char** ptr);
    short getShort(char** ptr);
    void interpreteAnswer();
};
} // namespace asteroids

#endif // CLIENT_H
