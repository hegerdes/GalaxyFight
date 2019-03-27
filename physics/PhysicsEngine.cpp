
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


void PhysicsEngine::updateAsteroidPos()
{
    if(client_global.updated_pos)
    {

        if(client_global.asteroid_ids.size() != client_global.asteroid_pos.size())
        {
            throw std::exception();
        }
        int arr_pos = 0;
        for(auto it : client_global.asteroid_ids)
        {
            for(auto b : m_objects)
            {
                if(it == b->getid())
                {
                    b->setPosition(client_global.asteroid_pos[arr_pos]);
                    break;
                }
            }
            arr_pos++;
        }
    }
}

void PhysicsEngine::updateBullets()
{
    if(client_global.updated_pos)
    {
        std::list<Bullet::Ptr> new_bullets;
        if(client_global.bullet_ids.size() != client_global.bullet_pos.size() || client_global.bullet_dirs.size() != client_global.bullet_ids.size())
        {
            throw std::exception();
        }
        int arr_pos = 0;
        bool existed;
        for(auto it : client_global.bullet_ids)
        {
            existed = false;
            for(auto b : m_bullets)
            {
                if(it == b->getid())
                {
                    b->setPosition(client_global.bullet_pos[arr_pos]);
                    b->setDirection(client_global.bullet_dirs[arr_pos]);
                    existed = true;
                    break;
                }


            }
            if(!existed)
            {
                Bullet::Ptr bp = Bullet::Ptr(new Bullet(client_global.bullet_pos[arr_pos], client_global.bullet_dirs[arr_pos]));
                bp->setid(it);
                std::cerr << "new id: " << it << "\n";
                new_bullets.push_back(bp);
            }
            arr_pos++;
        }

        //Adding missing bullets to bullets lists

        m_bullets.insert(m_bullets.end(), new_bullets.begin(), new_bullets.end());
        new_bullets.clear();
        client_global.bullet_ids.clear();
        client_global.bullet_pos.clear();
        client_global.bullet_dirs.clear();
        client_global.updated_pos = false;
    }
}

void PhysicsEngine::addDestroyable(Asteroid::Ptr& obj)
{
    m_objects.push_back(obj);
}

void PhysicsEngine::addBullet(Bullet::Ptr& bullet)
{
    //m_particles.addEffect(ParticleEffect::createBulletTail(bullet->getPosition(), bullet->direction(), bullet->lifetime()));

    m_bullets.push_back(bullet);
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
                m_particles.addEffect(ParticleEffect::createExplosionSphere((*it_2)->getPosition()));
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
    client_global.asteroids_deleted.clear();

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

void PhysicsEngine::process()
{
    //list<PhysicalObject::Ptr>::iterator p_it;
    list<Asteroid::Ptr>::iterator p_it;
    list<Bullet::Ptr>::iterator b_it;

    updateAsteroidPos();
    updateBullets();

    if(m_enemyPlayer->spaceCraftStatus() == 1)
    {
        m_particles.addEffect(ParticleEffect::createExplosionSphere(m_enemyPlayer->getPosition()));
        m_enemyPlayer->endDestruction();

    }

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
