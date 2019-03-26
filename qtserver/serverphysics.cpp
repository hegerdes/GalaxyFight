#include "serverphysics.h"

namespace asteroids {
    ServerPhysics::ServerPhysics()
    {
        m_timer = new Q_Timer();
        // Create a timer object to trigger the main loop
        connect(m_timer.get(), SIGNAL(timeout()), this, SLOT(updateData()));
    }

    void ServerPhysics::addBullet(ServerBullet::Ptr bullet){
        if(bullet->getId() > 0){
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
        m_timer->start(1000 / 30.0);
    }

    void ServerPhysics::updateData()
    {
        Removes removed = process();

    }

    void ServerPhysics::setParent(Server* serv)
    {
        m_server = serv;
    }

    Removes ServerPhysics::process(){
        Removes removed;

        //Asteroid Moving

        for(auto it : m_asteroids){
            it->move();
        }

        /*
         * COLLISION
         */

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
            }

            //Collision between second spacecraft and bullet
            if(m_second->collide(b))
            {
                b->m_alive = false;
                m_second->m_hp--;
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

                } else {
                    p_it++;
                }
            }

            // Check if bullet is dead. If it is, remove from
            // bullet list. Otherwise continue with next bullet.
            if (!b->m_alive)
            {
                removed.bullet_changed.push_back((*b_it)->getId());
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
            }

            //Collision between second spacecraft and bullet
            if(m_first->collide(b))
            {
                b->m_alive = false;
                m_first->m_hp--;
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
                }else{
                    o_it++;
                }
            }


            // Check if bullet is dead. If it is, remove from
            // bullet list. Otherwise continue with next bullet.
            if (!b->m_alive)
            {
                removed.bullet_changed.push_back((*b_it)->getId());
                b_it = m_second_bullets.erase(b_it);
            }
            else
            {
                b_it++;
            }
        }
    return removed;
    }
}
