#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "../math/Randomizer.hpp"
#include "../math/Vector.hpp"
#include "flags.h"
#include <QDebug>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtCore>
#include <QtNetwork>
#include <fstream>

#include "packettypes.h"
#include "loggingtype.h"


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

    /**
     * @brief The client_data struct represents the data that will be sent over the socket when a
     * 3D fight starts
     * @param position  The position of the fighter
     * @param x,y,z     Axis the axis of the fighter
     * @param shot  stores if the fighter shot a bullet or not
     * @param bullet_id     the id of each bullet that got shot
     * @param living    stores if the player is living or dead after he got shot
     * @param m_first_connect stores if this is the first connect of the client
     */
    struct client_data {
        //3D Teil
        asteroids::Vector<float> position;
        asteroids::Vector<float> xAxis;
        asteroids::Vector<float> yAxis;
        asteroids::Vector<float> zAxis;
        Bullet_shot shot;
        int bullet_id;
        Living living;

        //2D Teil
        std::string name = "Name undefined";
        bool m_first_connect = false;
    };

    /**
     * @brief user_data used to identify to which fighter/player the data to be transmitted belongs
     */
    client_data user_data_1;
    client_data user_data_2;


    void log(LoggingType type, std::string msg);

private:
    /**
     * @brief already_send containts information if a package has been already sent or not
     */
    bool already_send_1 = false;
    bool already_send_2 = false;


    /**
     * @brief the server manages the movement-direction and position of the asteroids that the client can see on his game
     * @param pos_astr position of the asteroids in the world represented as Vector3f
     * @param dir_astr direction of the asteroids in the world represented as Vector3f
     * @param size_astr the size of each asteroid in the world
     */
    asteroids::Vector3f pos_astr[10];
    asteroids::Vector3f dir_astr[10];
    float size_astr[10];


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
