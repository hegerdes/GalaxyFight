#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "../math/Randomizer.hpp"
#include "../math/Vector.hpp"
#include "../rendering/2D/PlanetChanges.hpp"
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

class Server : public QObject {
    Q_OBJECT
  public:
    explicit Server(QObject* parent = 0);

  signals:
    bool dataReceived(QByteArray);

  public slots:
    void newConnection();
    void disconnected();
    void readyRead();
    bool writeData(QByteArray const&);

  public:
    QByteArray data;
    QTcpServer* server;
    QHash<QTcpSocket*, QByteArray*> buffers; // We need a buffer to store data until block has completely
                                             // received
    QHash<QTcpSocket*, qint32*> sizes;       // We need to store the size to verify if
                                             // a block has received completely
    QByteArray test{"test_qstring"};
    // unsigned int count {0};
    // float count {0};
    // float factor {1};
    float position_temp[12];
    float position_temp_2[12];

    QTcpSocket* socket_1 = nullptr;
    QTcpSocket* socket_2 = nullptr;
    float getFloat(char** ptr);
    short getShort(char** ptr);
    char getChar(char** ptr);
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
       };

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

    client_data user_data_1;
    client_data user_data_2;

    void log(LoggingType type, std::string msg);
private:
    bool already_send_1 = false;
    bool already_send_2 = false;
    PacketType toSend;
    bool end3D;
    int winner;
    int pchanges_size1;
    int pchanges_size2;

    asteroids::Vector3f pos_astr[10];
    asteroids::Vector3f dir_astr[10];
    bool send_changes;
    std::vector<pchanges_data> pchanges_data1;
    std::vector<pchanges_data> pchanges_data2;
    std::vector<pchanges_data> outstanding_fights;
    std::vector<pchanges_data> pchanges_committ;
    // data from player that did not start the fight so
    // that we can take his data if he wins the 3d fight
    std::vector<pchanges_data> other_player_data;
    float size_astr[10];

    // sending packets
    void sendInit_3D(QByteArray& response, QTcpSocket* socket);
    void sendUpdate_3D_S(QByteArray& response, QTcpSocket* socket);
    void sendGame_Start(QByteArray &response, QTcpSocket *socket);
    void update_planet_changes();
    void sendUpdatedPlanetChanges();
    void sendEnd3D(QByteArray &response, QTcpSocket *socket);
    void recvPlanetChanges(char* data, QTcpSocket* socket);

    // receive packets
    void recvUpdate_3D_C(char* data, QTcpSocket* socket);
    void recvReady_T(char *data, QTcpSocket *socket);
};

#endif // MYTCPSERVER_H
