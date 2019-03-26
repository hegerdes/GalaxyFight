#ifndef SERVERPHYSICS_H
#define SERVERPHYSICS_H

#include "../math/Vector.hpp"
#include "mytcpserver.h"
#include <memory>
#include <list>
#include <vector>
#include "serverphysicalobject.h"

namespace asteroids {
    class ServerPhysics
    {
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
        void updateData();
        Removes process();

        QTimer m_timer;
        Server* m_server;
    };

    struct Removes {
        std::vector<int> bullet_changed;
        std::vector<int> aster_changed;
    };
}



#endif // SERVERPHYSICS_H
