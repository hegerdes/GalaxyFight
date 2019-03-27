
/*
 *  PhysicsEngine.cpp
 *
 *  @date 19.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */


#include "PhysicsEngine.hpp"
#include "global_socket.h"

#include <iostream>
using namespace std;

namespace asteroids
{


void PhysicsEngine::addDestroyable(Asteroid::Ptr& obj)
{
    m_objects.push_back(obj);
}

void PhysicsEngine::addBullet(Bullet::Ptr& bullet)
{
    //m_particles.addEffect(ParticleEffect::createBulletTail(bullet->getPosition(), bullet->direction(), bullet->lifetime()));

    m_bullets.push_back(bullet);
}


void PhysicsEngine::addEnemyBullet(Bullet::Ptr& bullet)
{
    m_particles.addEffect(ParticleEffect::createBulletTail(bullet->getPosition(), bullet->direction(), bullet->lifetime()));
    m_bullets_enemy.push_back(bullet);
}


int PhysicsEngine::removeAster()
{
    int n = 0;
    auto it = client_global.asteroids_deleted.begin();
    while(client_global.asteroids_deleted.end() != it)
    {
        std::cerr << "Aster No: " << *it << " wird gelöscht.\n";
        auto it_2 = m_objects.begin();
        while(m_objects.end() != it_2){
            if((*it_2)->getid() == *it){
                it_2 = m_objects.erase(it_2);
                std::cerr << "Aster No: " << *it << " wurde definitiv gelöscht.\n";
                n++;
                break;
            } else {
                it_2++;
            }
        }
        it++;
    }
    client_global.bullet_deleted.clear();

    return n;
}


int PhysicsEngine::removeBullets()
{
    int n = 0;
    auto it = client_global.bullet_deleted.begin();
    while(client_global.bullet_deleted.end() != it)
    {
        std::cerr << "Bullet No: " << *it << " wird gelöscht.\n";
        auto it_2 = m_bullets.begin();
        while(m_bullets.end() != it_2){
            if((*it_2)->getid() == *it){
                it_2 = m_bullets.erase(it_2);
                std::cerr << "Bullet No: " << *it << " wurde definitiv gelöscht.\n";
                n++;
                break;
            } else {
                it_2++;
            }
        }

        it_2 = m_bullets_enemy.begin();
        while(m_bullets_enemy.end() != it_2){
            if((*it_2)->getid() == *it){
                it_2 = m_bullets_enemy.erase(it_2);
                std::cerr << "Bullet No: " << *it << " wurde definitiv gelöscht.\n";
                n++;
                break;
            } else {
                it_2++;
            }
        }

        it++;
    }
    client_global.bullet_deleted.clear();

    return n;

    /*
    while(client_global.bullet_deleted.end() != it)
    {
        list<Bullet::Ptr>::iterator b_it;
        b_it = m_bullets.begin();
        std::cerr << __LINE__ << "\n";
        while(b_it != m_bullets.end() && !deleted)
        {
            std::cerr << __LINE__ << "\n";
            Bullet::Ptr b = *b_it;
            std::cerr << __LINE__ << "\n";
            if(it == client_global.bullet_deleted.end())
            {
                std::cerr<<"HAHAH\n";
            }
            int temp = *it;
            if(b->getid() == temp)
            {
                b_it = m_bullets.erase(b_it);
                n++;
                it = client_global.bullet_deleted.erase(it);
                deleted = true;
            } else
            {
                b_it++;
            }
            std::cerr << __LINE__ << "\n";
            if(it == client_global.bullet_deleted.end())
            {
                std::cerr<<"HAHAH\n";
            }

            std::cerr << __LINE__ << "\n";

        }
        if(deleted)
        {
            deleted = false;
        } else
        {
            it++;
        }

        std::cerr << __LINE__ << "\n";
    }
    std::cerr << __LINE__ << "\n";
    while(client_global.bullet_deleted.end() != it)
    {
        list<Bullet::Ptr>::iterator b_it;
        b_it = m_bullets_enemy.begin();
        while(b_it != m_bullets.end())
        {
            Bullet::Ptr b = *b_it;
            if(b->getid() == *it)
            {
                b_it = m_bullets_enemy.erase(b_it);
                n++;
                it = client_global.bullet_deleted.erase(it);
            } else
            {
                b_it++;
            }
        }
    }*/
    //std::cerr << __LINE__ << "\n";
    //return n;
}

void PhysicsEngine::process()
{
    //list<PhysicalObject::Ptr>::iterator p_it;
    list<Asteroid::Ptr>::iterator p_it;
    list<Bullet::Ptr>::iterator b_it;

    if(client_global.bullet_deleted.size() != removeBullets())
    {
        std::cerr << "Nicht alle Bullets removed;" << __LINE__;
    }
    if(client_global.asteroids_deleted.size() != removeAster())
    {
        std::cerr << "Nicht alle asteroiden gelöscht" << __LINE__;
    }
    // Move all objects
    for (p_it = m_objects.begin(); p_it != m_objects.end(); p_it++)
    {
        Asteroid::Ptr p = *p_it;
        p->move();
    }

    //Move bullets and test for hits
    b_it = m_bullets.begin();
    while (b_it != m_bullets.end())
    {
        Bullet::Ptr b = *b_it;
        b->run();
        //std::cerr << "Own Bullets" << b->getid() << "\n";

        //TODO: Einkommentieren, wenn merge; m_enemy_spacecraft noch uninitialisiert
        /*
         *check of enemy Ship with Bullets*/
        /*
        if(m_enemyPlayer->collideShip(b->getPosition(), b->radius()))
        {
            b->destroy();

            m_enemyPlayer->gotHit();

            m_particles.addEffect(ParticleEffect::createExplosionSphere(b->getPosition()));
        }



        // Check for collisions with present objects
        p_it = m_objects.begin();
        while (p_it != m_objects.end())
        {
            if ((*p_it)->collision(b->getPosition(), b->radius()))
            {
                // Mark bulled as killed
                b->destroy();

                // Delete destroyed object
                p_it = m_objects.erase(p_it);

                // Add explosion
                m_particles.addEffect(ParticleEffect::createExplosionSphere(b->getPosition()));
            } else{
                p_it++;
            }
        }*/

        // Check if bullet is dead. If it is, remove from
        // bullet list. Otherwise continue with next bullet.
        if (!b->alive())
        {
            b_it = m_bullets.erase(b_it);
        }
        else
        {
            b_it++;
        }
    }
    //Gibt dem Raumschiff eine Explosion beim verschwinden
    if(m_spacecraft->spaceCraftStatus() == 1)
    {
        m_particles.addEffect(ParticleEffect::createExplosionSphere(m_spacecraft->getPosition()));
        m_spacecraft->endDestruction();
    }
    

    if(m_enemyPlayer->spaceCraftStatus() == 1)
    {
        m_particles.addEffect(ParticleEffect::createExplosionSphere(m_enemyPlayer->getPosition()));
        m_enemyPlayer->endDestruction();

    }

    //Move bullets and test for hits
    b_it = m_bullets_enemy.begin();
    while (b_it != m_bullets_enemy.end())
    {
        Bullet::Ptr b = *b_it;
        b->run();
        //std::cerr << "Enemy Bullets" << b->getid() << "\n";


        /*check of Ship with enemy Bullets*/
        /*
        if(m_spacecraft->collideShip(b->getPosition(), b->radius()))
        {
            b->destroy();

            m_spacecraft->gotHit();

            m_particles.addEffect(ParticleEffect::createExplosionSphere(b->getPosition()));
        }

        // Check for collisions with present objects
        p_it = m_objects.begin();
        while (p_it != m_objects.end())
        {
            if ((*p_it)->collision(b->getPosition(), b->radius()))
            {
                // Mark bulled as killed
                b->destroy();

                // Delete destroyed object
                p_it = m_objects.erase(p_it);

                // Add explosion
                m_particles.addEffect(ParticleEffect::createExplosionSphere(b->getPosition()));
            }
            p_it++;
        }
        */
        // Check if bullet is dead. If it is, remove from
        // bullet list. Otherwise continue with next bullet.
        if (!b->alive())
        {
            b_it = m_bullets_enemy.erase(b_it);
        }
        else
        {
            b_it++;
        }
    }


    /*
     * Kollisionstest zwischen Asteroiden und Schiffen
     * es werden die jeweiligen gotHit() operationen der Schiffe aufgerufen
     */
    /*
    p_it = m_objects.begin();
    while(p_it != m_objects.end())
    {
        if(m_spacecraft->collideShip((*p_it)->getPosition(), (*p_it)->getradius()))
        {
            m_particles.addEffect(ParticleEffect::createExplosionSphere((*p_it)->getPosition()));
            m_spacecraft->gotHit();

            p_it = m_objects.erase(p_it);
        }*/
        /*
         * TODO:: Einkommentieren, wenn merge, dann m_enemy_ship initialisiert
        */
    /*
        else if(m_enemyPlayer->collideShip((*p_it)->getPosition(), (*p_it)->getradius()))
        {
            m_particles.addEffect(ParticleEffect::createExplosionSphere((*p_it)->getPosition()));
            m_enemyPlayer->gotHit();

            p_it = m_objects.erase(p_it);
        }else
        {
            p_it++;
        }
    }*/


    m_particles.update();

}

void PhysicsEngine::render()
{
   
    // Render all objects
    list<Asteroid::Ptr>::iterator p_it;
    list<Bullet::Ptr>::iterator b_it;
    list<Bullet::Ptr>::iterator be_it;
 

     for(p_it = m_objects.begin(); p_it != m_objects.end(); p_it++)
        {
            Asteroid::Ptr p = *p_it;
            p->render();
        }

    // Render active bullets and delete inactive ones
    b_it = m_bullets.begin();
    while(b_it != m_bullets.end())
    {
        Bullet::Ptr b = (*b_it);
        b->render();
        b_it++;
    }



    // Render active bullets and delete inactive ones
    be_it = m_bullets_enemy.begin();
    while(be_it != m_bullets_enemy.end())
    {
        Bullet::Ptr b = (*be_it);
        b->render();
        be_it++;
    }

    m_particles.render();
    //cout << m_bullets.size() << endl;

}

void PhysicsEngine::addSpaceCraft(SpaceCraft::Ptr spacecraft){
    m_spacecraft = spacecraft;
}

void PhysicsEngine::addEnemyPlayer(SpaceCraft::Ptr enemy){
    m_enemyPlayer = enemy;
}

} /* namespace asteroids */
