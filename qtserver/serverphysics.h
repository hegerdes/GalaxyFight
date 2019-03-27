#ifndef SERVERPHYSICS_H
#define SERVERPHYSICS_H

#include "../math/Vector.hpp"
#include <memory>
#include <list>
#include <vector>
#include <QObject>
#include <QTimer>
#include "serverphysicalobject.h"

#include "client_data.h"

namespace asteroids {

    struct Removes {
        std::vector<int> bullet_changed;
        std::vector<int> aster_changed;

        int health_socket_1;
        int health_socket_2;
    };

    class ServerPhysics : public QObject
    {
        Q_OBJECT
    private:
        std::list<ServerBullet::Ptr> m_first_bullets;
        std::list<ServerBullet::Ptr> m_second_bullets;
        std::list<ServerAsteroid::Ptr> m_asteroids;

        ServerSpacecraft::Ptr m_first;
        ServerSpacecraft::Ptr m_second;

        std::vector<int> deleted_asteroids_ids_1;
        std::vector<int> deleted_asteroids_ids_2;
        std::vector<int> deleted_bullets_ids_1;
        std::vector<int> deleted_bullets_ids_2;
    public:
        ServerPhysics() = delete;
        ServerPhysics(client_data &data_socket_1, client_data &data_socket_2);

        void setParent(client_data &data_socket_1, client_data &data_socket_2);
        void addBullet(ServerBullet::Ptr bullet);
        void addAsteroid(ServerAsteroid::Ptr aster);
        void run();
        Removes process();

        std::shared_ptr<QTimer> m_timer;
        client_data &m_data_socket_1;
        client_data &m_data_socket_2;
        std::vector<int> getDelAstr1();
        std::vector<int> getDelBullets1();
        std::vector<int> getDelAstr2();
        std::vector<int> getDelBullets2();
    public Q_SLOTS:
        void updateData();
    };

}



#endif // SERVERPHYSICS_H
