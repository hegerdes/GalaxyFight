#ifndef SERVERPHYSICALOBJECT_H
#define SERVERPHYSICALOBJECT_H

#include "../math/Vector.hpp"
#include <memory>

namespace asteroids {
    class ServerPhysicalObject
    {
        public:
            using Ptr = std::shared_ptr<ServerPhysicalObject>;
            bool m_alive = true;
            asteroids::Vector3f m_pos;

            asteroids::Vector3f m_dir;
            float m_speed;
            float m_radius;
            int m_id;

            ServerPhysicalObject(asteroids::Vector3f pos, asteroids::Vector3f dir, float speed, float radius, int id);
            bool collide(Ptr other);
            void move();
            int getId(){
                return m_id;
            }
    };

    class ServerBullet : public ServerPhysicalObject{
        public:
            using Ptr = std::shared_ptr<ServerBullet>;
            ServerBullet(asteroids::Vector3f pos, asteroids::Vector3f dir, int id, float speed = 10, float radius = 10):
                ServerPhysicalObject (pos, dir, speed, radius, id){}
            int m_livetime = 300;

    };

    class ServerAsteroid : public ServerPhysicalObject{
        public:
            using Ptr = std::shared_ptr<ServerAsteroid>;
            ServerAsteroid(asteroids::Vector3f pos, asteroids::Vector3f dir, float speed, float radius, int id):
                ServerPhysicalObject (pos, dir, speed, radius, id){}
    };

    class ServerSpacecraft {

    public:
            Vector3f m_xAxis;
            Vector3f m_yAxis;
            Vector3f m_zAxis;
            Vector3f m_pos;

            using Ptr = std::shared_ptr<ServerSpacecraft>;
            int m_hp = 10;
            ServerSpacecraft() = default;
            ServerSpacecraft(Vector3f xAxis, Vector3f yAxis, Vector3f zAxis, Vector3f pos);
            void update(Vector3f xAxis, Vector3f yAxis, Vector3f zAxis, Vector3f pos);
            bool collide(ServerPhysicalObject::Ptr other);
    };

}
#endif // SERVERPHYSICALOBJECT_H
