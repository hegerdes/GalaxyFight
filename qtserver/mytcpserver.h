#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "../math/Randomizer.hpp"
#include "../math/Vector.hpp"
#include "../view/items/PlanetChanges.hpp"
#include "flags.h"
#include <QDebug>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtCore>
#include <QtNetwork>
#include <fstream>
#include <vector>

#include "packettypes.h"
#include "loggingtype.h"
#include "serverphysics.h"
#include "serverphysicalobject.h"

#include "client_data.h"


/**
 * @brief represents the server that handles our global clients via QTcpSockets. We are using our own protocol to interpret each
 * packet that we send
 */

class Server : public QObject {
    Q_OBJECT
  public:
    explicit Server(QObject* parent = 0);

  signals:
    /**
     * @brief dataReceived the data we recive from client. This function calls interpreteAnswer() to interpret each packet recived
     * @return sucess if data got recived
     */
    bool dataReceived(QByteArray);

  public slots:
    /**
     * @brief newConnection connects the client to the server -> max two connections possible if connections < 2 the next one will
     * be declined
     */
    void newConnection();

    /**
     * @brief disconnects client from server and deletes socket
     */
    void disconnected();
    void readyRead();
    bool writeData(QByteArray const&);

  public:
    /**
     * @brief data to be transmitted to the client
     */
    QByteArray data;
    QTcpServer* server;

    /**
     * @brief socket_1 and socket_2 are the sockets to transmit data to the clients using Tcp.
     * if only one socket is connected the game will not start until another one connects to the server
     */
    QTcpSocket* socket_1 = nullptr;
    QTcpSocket* socket_2 = nullptr;

    /**
     * @brief buffers stored the data until a data block has been completly recived
     */
    QHash<QTcpSocket*, QByteArray*> buffers;

    /**
     * @brief sizes need to be stroed to verify if a block has been recived compelety
     */
    QHash<QTcpSocket*, qint32*> sizes;

    QByteArray test{"test_qstring"};

    /**
     * @brief position_temp represents m_position of the SpaceCraft with x,y and z-axis
     */
    float position_temp[12];
    float position_temp_2[12];

    /**
     * @brief getFloat increments the pointer for 4 bytes
     * @param ptr the pointer to be incremented
     * @return the value of data in pointer
     */
    float getFloat(char** ptr);

    /**
     * @brief getShort increments the pointer for 1 bytes
     * @param ptr the pointer to be incremented
     * @return the value of data in pointer
     */
    short getShort(char** ptr);

    /**
     * @brief getChar increments the pointer for 1 bytes
     * @param ptr the pointer to be incremented
     * @return the value of data in pointer
     */
    char getChar(char** ptr);

    /**
     * @brief getInt increments the pointer for 4 bytes
     * @param ptr the pointer to be incremented
     * @return the value of data in pointer
     */
    int getInt(char** ptr);

    struct pchanges_data{
           asteroids::PlanetChanges::Owner m_own;
           int m_id;
           int m_num_of_ore;
           int num_factory;
           int num_mine;
           int num_fighters;
           int num_transporter;
           bool m_attack_planet;
           int stored_ore;
       };


    client_data user_data_1;
    client_data user_data_2;


    void log(LoggingType type, std::string msg);

private:
    /**
     * @brief already_send containts information if a package has been already sent or not
     */
    bool already_send_1 = false;
    bool already_send_2 = false;
    bool end3D;
    int winner;
    int pchanges_size1;
    int pchanges_size2;
    bool m_socket_1_pchange_received = false;
    bool m_socket_2_pchange_received = false;
    bool m_player1_end_3d_received = false;
    bool m_player2_end_3d_received = false;
    bool m_socket_1_pchanges_commit_deletable = false;
    bool m_socket_2_pchanges_commit_deletable = false;


    /**
     * @brief the server manages the movement-direction and position of the asteroids that the client can see on his game
     * @param pos_astr position of the asteroids in the world represented as Vector3f
     * @param dir_astr direction of the asteroids in the world represented as Vector3f
     * @param size_astr the size of each asteroid in the world
     */
    asteroids::Vector3f pos_astr[10];
    asteroids::Vector3f dir_astr[10];
    bool send_changes;
    std::vector<pchanges_data> pchanges_data1;
    std::vector<pchanges_data> pchanges_data2;
    std::vector<pchanges_data> player1_outstanding_fights;
    std::vector<pchanges_data> pchanges_committ;
    // data from player that did not start the fight so
    // that we can take his data if he wins the 3d fight
    std::vector<pchanges_data> player2_outstanding_fights;
    float size_astr[10];

    asteroids::ServerPhysics physics;

    // sending packets

    /**
     * @brief toSend the type of Packet from Packettypes to be send to the reciver (default is the update packet in 3d mode)
     */
    PacketType toSend;

    /**
     * @brief functions to build the packets we want to send to the client via Tcp
     * @param response the array we are writing the data that will be transmitted
     * @param socket the client to which the data will be transmitted
     */

    /**
     * @brief sendInit_3D sends the 3D packet to initalize the position of the player and the asteroids
     */
    void sendInit_3D(QByteArray& response, QTcpSocket* socket);
    /**
     * @brief sendUpdate_3D_S sends the Update package in 3D mode so that the client can update the enemy positions and status
     */
    void sendUpdate_3D_S(QByteArray& response, QTcpSocket* socket);
    /**
     * @brief sendGame_Start signializes the client that the game has started.Player id  will be sent with the package
     */
    void sendGame_Start(QByteArray &response, QTcpSocket *socket);
    void update_planet_changes();
    void sendUpdatedPlanetChanges();
    void sendEnd3D(QByteArray &response, QTcpSocket *socket);
    void recvPlanetChanges(char* data, QTcpSocket* socket);
    void recv_end_3D(char* data, QTcpSocket* socket);
    void resend_UpdatedPlanetChanges(QTcpSocket* socket);
    void resend_init_3d(QTcpSocket* socket);

    /**
     * @brief functions to recive packets that  the client can send to the server
     * once the server recives a packet readyRead() is called and the server interpetes the package he got sent and calls the
     * suiting function
     * @param data the data that will be read
     * @param socket the socket where the message came from
     */

    /**
     * @brief recvUpdate_3D_C recives the SpaceCraft updates from the client so that the information can be sent
     * to the enemy player with Update_3D_S
     */
    void recvUpdate_3D_C(char* data, QTcpSocket* socket);
    /**
     * @brief recvReady_T Server waits for this packet from both players so that it can transmit the sendGame_Start package
     */
    void recvReady_T(char *data, QTcpSocket *socket);
};

#endif // MYTCPSERVER_H
