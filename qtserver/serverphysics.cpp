#include "serverphysics.h"
#include <QTimer>

#include "serverphysicalobject.h"

namespace asteroids {

    ServerPhysics::ServerPhysics(client_data &data_socket_1, client_data &data_socket_2) :
        m_timer(new QTimer()),
        m_data_socket_1(data_socket_1),
        m_data_socket_2(data_socket_2),
        m_first(new ServerSpacecraft()),
        m_second(new ServerSpacecraft())
    {
        // Create a timer object to trigger the main loop
        connect(m_timer.get(), SIGNAL(timeout()), this, SLOT(updateData()));
    }

    void ServerPhysics::addBullet(ServerBullet::Ptr bullet){
        std::cerr << "\n\n\n BULLET ANGEKOMMEN MIT ID: " << bullet->getId() <<"\n\n\n";
        if(bullet->getId() < 0){
            m_first_bullets.push_back(bullet);
        } else {
            m_second_bullets.push_back(bullet);
        }
    }

    void ServerPhysics::addAsteroid(ServerAsteroid::Ptr aster){
        m_asteroids.push_back(aster);
    }

    void ServerPhysics::run()
    {
        m_timer->start(1000 / 60.0);
    }

    void ServerPhysics::updateData()
    {
        Removes removed = process();
        //append removed asteroids to changes list from struct

        for(int i : removed.aster_changed)
        {
            deleted_asteroids_ids_1.push_back(i);
            deleted_asteroids_ids_2.push_back(i);
        }

        //Append deleted bullets to each list.
        for(int i : removed.bullet_changed)
        {
            deleted_bullets_ids_1.push_back(i);
            deleted_bullets_ids_2.push_back(i);
        }


    }

    //TODO COPY AND CLEAR ORIGINAL
    std::vector<int> ServerPhysics::getDelAstr1(){
        std::vector<int> nid(deleted_asteroids_ids_1);
        deleted_asteroids_ids_1.clear();
        return nid;
    }

    std::vector<int> ServerPhysics::getDelBullets1(){
        std::vector<int> nid(deleted_bullets_ids_1);
        deleted_bullets_ids_1.clear();
        return nid;
    }

    std::vector<int> ServerPhysics::getDelAstr2(){
        std::vector<int> nid(deleted_asteroids_ids_2);
        deleted_asteroids_ids_2.clear();
        return nid;
    }

    std::vector<int> ServerPhysics::getDelBullets2(){
        std::vector<int> nid(deleted_bullets_ids_2);
        deleted_bullets_ids_2.clear();
        return nid;
    }

    Removes ServerPhysics::process(){

        //Update positions of spacecrafts
        m_first->m_xAxis = m_data_socket_1.xAxis;
        m_first->m_yAxis = m_data_socket_1.yAxis;
        m_first->m_zAxis = m_data_socket_1.zAxis;
        m_first->m_pos = m_data_socket_1.position;

        /*
        m_first->update(m_data_socket_1.xAxis, m_data_socket_1.yAxis, m_data_socket_1.zAxis, m_data_socket_1.position);
        m_second->update(m_data_socket_2.xAxis, m_data_socket_2.yAxis, m_data_socket_2.zAxis, m_data_socket_2.position);
*/
        m_second->m_xAxis = m_data_socket_2.xAxis;
        m_second->m_yAxis = m_data_socket_2.yAxis;
        m_second->m_zAxis = m_data_socket_2.zAxis;
        m_second->m_pos = m_data_socket_2.position;

        ////std::cerr << "Position of 1: " << m_first->m_pos << "\nPosition of 2: " << m_second->m_pos << "\n";

        Removes removed;

        //Asteroid Moving

        for(auto it : m_asteroids){
            it->move();
            /*
            if(it->getId() == 1){
                //std::cerr << "Asteroid 1 moved: |||" << it->m_pos << "|||";
                //std::cerr << "speed: Direction: " << it->m_speed << "|||" << it->m_dir;
            }
            */
        }

        /*
         * COLLISION
         */


        /*
         * Kollisionstest zwischen Asteroiden und Schiffen
         * es werden die jeweiligen gotHit() operationen der Schiffe aufgerufen
         */
        auto p_it = m_asteroids.begin();
        while(p_it != m_asteroids.end())
        {
            asteroids::ServerAsteroid::Ptr ast = *p_it;

            if(m_first->collide(ast))
            {
                m_first->m_hp--;

                removed.aster_changed.push_back(ast->getId());

                p_it = m_asteroids.erase(p_it);
                ////std::cerr << "Hit between Spacecraft Socket 1 and Asteroid\n";
            }
            else if(m_second->collide(ast))
            {
                m_second->m_hp--;

                removed.aster_changed.push_back(ast->getId());

                p_it = m_asteroids.erase(p_it);
//              //std::cerr << "Hit between Spacecraft Socket 2 and Asteroid\n";
            }else
            {
                p_it++;
            }
        }

        //spacecraft1 bullet2 interaction + asteroids
        //Move bullets and test for hits
        auto b_it = m_first_bullets.begin();
        while (b_it != m_first_bullets.end())
        {
            //Moving
            ServerBullet::Ptr b = *b_it;
            b->move();
            b->m_livetime--;
            if(b->m_livetime == 0){
                b->m_alive = false;
//                //std::cerr << "bullet died1 " << (*b_it)->getId() << "\n";
            }

            //Collision between second spacecraft and bullet
            if(m_second->collide(b))
            {
                b->m_alive = false;
                m_second->m_hp--;
                removed.health_socket_2 = m_second->m_hp;
//                //std::cerr << "Hit between Spacecraft 2 and Bullet\n";
            }

            // Check for collisions with present objects
            auto p_it = m_asteroids.begin();
            while (p_it != m_asteroids.end())
            {
                if ((*p_it)->collide(b))
                {
                    // Mark bulled as killed
                    b->m_alive = false;

                    // Delete destroyed object
                    removed.aster_changed.push_back((*p_it)->getId());
                    p_it = m_asteroids.erase(p_it);

//                    //std::cerr << "Hit between Bullet and Asteroid\n";

                } else {
                    p_it++;
                }
            }

            // Check if bullet is dead. If it is, remove from
            // bullet list. Otherwise continue with next bullet.
            if (!b->m_alive)
            {
                removed.bullet_changed.push_back((*b_it)->getId());
                ////std::cerr << "bullet removed1 " << (*b_it)->getId() << "\n";
                ////std::cerr << "BULLET POS 1: " << (*b_it)->m_pos << "\n";
                b_it = m_first_bullets.erase(b_it);
            }
            else
            {
                b_it++;
            }
        }
        //spacecraft2 bullet1 interaction + asteroids

        b_it = m_second_bullets.begin();
        while (b_it != m_second_bullets.end())
        {
            //Moving
            ServerBullet::Ptr b = *b_it;
            b->move();
            b->m_livetime--;
            if(b->m_livetime == 0){
                b->m_alive = false;
                ////std::cerr << "bullet died2 " << (*b_it)->getId() << "\n";
            }

            //Collision between second spacecraft and bullet
            if(m_first->collide(b))
            {
                b->m_alive = false;
                m_first->m_hp--;
                removed.health_socket_1 = m_first->m_hp;
                ////std::cerr << "Hit between Spacecraft 1 and Bullet\n";
            }

            // Check for collisions with present objects
            auto p_it = m_asteroids.begin();
            while (p_it != m_asteroids.end())
            {
                if ((*p_it)->collide(b))
                {
                    // Mark bulled as killed
                    b->m_alive = false;

                    // Delete destroyed object
                    removed.aster_changed.push_back((*p_it)->getId());
                    p_it = m_asteroids.erase(p_it);
                    ////std::cerr << "Hit between Bullet and Asteroid\n";
                } else {
                    p_it++;
                }
            }

            //Check Bullet Bullet collision
            auto o_it = m_first_bullets.begin();
            while(o_it != m_first_bullets.end()){
                if((*o_it)->collide(b)){
                    b->m_alive  = false;
                    (*o_it)->m_alive = false;
                    removed.bullet_changed.push_back((*o_it)->getId());
                    o_it = m_first_bullets.erase(o_it);
                    ////std::cerr << "Hit between Bullet and Bullet\n";
                }else{
                    o_it++;
                }
            }


            // Check if bullet is dead. If it is, remove from
            // bullet list. Otherwise continue with next bullet.
            if (!b->m_alive)
            {
                removed.bullet_changed.push_back((*b_it)->getId());
                ////std::cerr << "bullet removed2 " << (*b_it)->getId() << "\n";
                ////std::cerr << "BULLET POS 2: " << (*b_it)->m_pos << "\n";
                b_it = m_second_bullets.erase(b_it);
            }
            else
            {
                b_it++;
            }
        }
        removed.health_socket_1 = m_first->m_hp;
        removed.health_socket_2 = m_second->m_hp;
    return removed;
    }
    
    void resetPhysics()
    {
        m_first = ServerSpacecraft::Ptr(new ServerSpacecraft());
        m_second = ServerSpacecraft::Ptr(new ServerSpacecraft();
        m_first_bullets.clear();
        m_second_bullets.clear();
        m_asteroids.clear();
        deleted_asteroids_ids_1.clear();
        deleted_asteroids_ids_2.clear();
        deleted_bullets_ids_1.clear();
        deleted_bullets_ids_2.clear();
    }

    void pause()
    {
        m_timer.stop();
    }
}
