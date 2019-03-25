#include "serverphysicalobject.h"

namespace asteroids {
    ServerPhysicalObject::ServerPhysicalObject(Vector3f pos, Vector3f dir, float speed, float radius, int id) :
        m_pos(pos),
        m_dir(dir),
        m_speed(speed),
        m_radius(radius),
        m_id(id)
    {}

    void ServerPhysicalObject::move(){
        m_pos = m_pos + m_dir * m_speed;
    }

    bool ServerPhysicalObject::collide(Ptr other){
        using std::pow;
        Vector<float> dist = m_pos - other->m_pos;
        return (pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2)) <= pow((m_radius + other->m_radius), 2);
    }
}
