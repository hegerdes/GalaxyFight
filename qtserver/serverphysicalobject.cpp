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

    bool ServerSpacecraft::collide (ServerPhysicalObject::Ptr other){
        if(m_hp == 0)
        {
            return false;
        }

        Vector<float> xAxis = m_xAxis;
        Vector<float> yAxis = m_yAxis;

        Vector<float> pos = m_pos;
        Vector<float> vec = other->m_pos;

        float r = other->m_radius;

        Vector<float> dist = pos - vec;

        Vector<float> center_box = pos;
        dist = center_box - vec;

        float distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
        if(distance <= pow(r + 50, 2))
        {
            std::cout << "CENTER HIT" << std::endl;
            return true;
        }

        Vector<float> back_box = pos + xAxis * 90;
        dist = back_box - vec;
        distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
        if(distance <= pow(r + 50, 2))
        {
            std::cout << "BACK HIT" << std::endl;
            return true;
        }

        Vector<float> back_box_left = back_box + yAxis * 70;
        dist = back_box_left - vec;
        distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
        if(distance <= pow(r + 55, 2))
        {
            std::cout << "BACK LEFT HIT" << std::endl;
            return true;
        }

        Vector<float> back_box_right = back_box - yAxis * 70;
        dist = back_box_right - vec;
        distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
        if(distance <= pow(r + 55, 2))
        {
            std::cout << "BACK RIGHT HIT" << std::endl;
            return true;
        }

        Vector<float> farback_box = pos + xAxis * 110;
        dist = farback_box - vec;
        distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
        if(distance <= pow(r + 50, 2))
        {
            std::cout << "FAR-BACK HIT" << std::endl;
            return true;
        }

        Vector<float> farfront_box = pos - xAxis * 70;
        dist = farfront_box - vec;
        distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
        if(distance <= pow(r + 50, 2))
        {
            std::cout << "FRONT HIT" << std::endl;
            return true;
        }

        Vector<float> front_box = pos - xAxis * 100;
        dist = front_box - vec;
        distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
        if(distance <= pow(r + 50, 2))
        {
            std::cout << "FAR-FRONT HIT" << std::endl;
            return true;
        }
        return false;
    }
}
