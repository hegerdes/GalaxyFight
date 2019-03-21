
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

#include <iostream>
using namespace std;

namespace asteroids
{


PhysicsEngine::PhysicsEngine(SpaceCraft::Ptr o_spacecraft, SpaceCraft::Ptr o_enemy)
{
    m_spacecraft = SpaceCraft::Ptr(o_spacecraft);
    m_enemy_spacecraft = SpaceCraft::Ptr(o_enemy);
}

void PhysicsEngine::addDestroyable(PhysicalObject::Ptr& obj)
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


void PhysicsEngine::process()
{
    //list<PhysicalObject::Ptr>::iterator p_it;
    list<PhysicalObject::Ptr>::iterator p_it;
    list<Bullet::Ptr>::iterator b_it;

    // Move all objects
    for (p_it = m_objects.begin(); p_it != m_objects.end(); p_it++)
    {
        PhysicalObject::Ptr p = *p_it;
        p->move();
    }

    //Move bullets and test for hits
    b_it = m_bullets.begin();
    while (b_it != m_bullets.end())
    {
        Bullet::Ptr b = *b_it;
        b->run();

        //TODO: Einkommentieren, wenn merge; m_enemy_spacecraft noch uninitialisiert
        /*
         *check of enemy Ship with Bullets*/

        if(m_enemy_spacecraft->collideShip(b->getPosition(), b->radius()))
        {
            b->destroy();

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


        /*check of Ship with enemy Bullets*/

        if(m_spacecraft->collideShip(b->getPosition(), b->radius()))
        {
            b->destroy();

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
    p_it = m_objects.begin();
    while(p_it != m_objects.end())
    {
        if(m_spacecraft->collideShip((*p_it)->getPosition(), (*p_it)->getradius()))
        {
            m_particles.addEffect(ParticleEffect::createExplosionSphere((*p_it)->getPosition()));
            m_spacecraft->gotHit();

            p_it = m_objects.erase(p_it);
        }
        /*
         * TODO:: Einkommentieren, wenn merge, dann m_enemy_ship initialisiert
        */
        else if(m_enemy_spacecraft->collideShip((*p_it)->getPosition(), (*p_it)->getradius()))
        {
            m_particles.addEffect(ParticleEffect::createExplosionSphere((*p_it)->getPosition()));
            m_enemy_spacecraft->gotHit();

            p_it = m_objects.erase(p_it);
        }else
        {
            p_it++;
        }
    }

    m_particles.update();

}

void PhysicsEngine::render()
{
   
    // Render all objects
    list<PhysicalObject::Ptr>::iterator p_it;
    list<Bullet::Ptr>::iterator b_it;
    list<Bullet::Ptr>::iterator be_it;
 

     for(p_it = m_objects.begin(); p_it != m_objects.end(); p_it++)
        {
            PhysicalObject::Ptr p = *p_it;
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
