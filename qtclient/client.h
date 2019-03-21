#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include <iostream>
#include <view/math/Vector.hpp>
#include <qtserver/packettypes.h>
#include <qtserver/flags.h>
#include <vector>

namespace asteroids{
    class Client
    {
        public:
            Client();

            //init_3D
            Vector<float> ownPos;
            Vector<float> ownxAxis;
            Vector<float> ownyAxis;
            Vector<float> ownzAxis;

            //update_3D_S / init_3D
            Vector<float> enemyPos;
            Vector<float> enemyxAxis;
            Vector<float> enemyyAxis;
            Vector<float> enemyzAxis;
            Bullet_shot enemy_shot;
            int enemy_shot_id;

            std::vector<int> asteroids_deleted;
            std::vector<int> bullet_deleted;
            Hit own_hit;

            //end_3D
            char winner_no;
            //start_2D
            void sendUpdate_3D_C(Vector<float> pos, Vector<float> xAxis, Vector<float> yAxis, Vector<float> zAxis, Bullet_shot shot, Living living, int bullet_id);
            void readData();
            void wait_for_readData();

            void connect(QString addr, quint16 port);
            bool init_received;
    private:
            void writeData(QByteArray const & data);
            void init_3d(char *);
            void update_3D_S(char *);

            QTcpSocket socket;
            float getFloat(char **ptr);
            int getInt(char **ptr);
            char getChar(char **ptr);
            short getShort(char **ptr);
    };
}

#endif // CLIENT_H
