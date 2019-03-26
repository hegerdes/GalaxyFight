#ifndef SERVERPHYSICS_H
#define SERVERPHYSICS_H

#include "../math/Vector.hpp"
#include "mytcpserver.h"
#include <memory>
#include <list>
#include <vector>
#include "serverphysicalobject.h"

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
    public:
        ServerPhysics();

        void setParent(Server* serv);
        void addBullet(ServerBullet::Ptr bullet);
        void addAsteroid(ServerAsteroid::Ptr aster);
        void run();
        Removes process();

        std::shared_ptr<QTimer> m_timer;
        Server* m_server;
    public Q_SLOTS:
        void updateData();
    };
}



#endif // SERVERPHYSICS_H
