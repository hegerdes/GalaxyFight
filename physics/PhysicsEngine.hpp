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


#ifndef PHYSICSENGINE_HPP_
#define PHYSICSENGINE_HPP_

#include <list>
#include <memory>

#include "PhysicalObject.hpp"
#include "ParticleEngine.hpp"

#include "rendering/Bullet.hpp"
#include "rendering/SpaceCraft.hpp"
#include "rendering/Asteroid.hpp"


using std::list;


namespace asteroids
{

/**
 * @brief   Class that calculates the position of all present objects
 *          and implements collision detection. Handles particle
 *          effects like explosions.
 */
class PhysicsEngine : public Renderable
{
public:

    using Ptr = std::shared_ptr<PhysicsEngine>;

    /**
     * @brief   Ctor.
     */
    PhysicsEngine() = default;

    /**
     * @brief   Dtor.
     */
    virtual ~PhysicsEngine() = default;

    /**
     * @brief   Adds a destroyable objects, i.e. a static objects
     *          that can be hit by a bullet (asteroids etc.). Takes ownership of 
     *          the given pointer
     */
    void addDestroyable(Asteroid::Ptr& d);

    void updateBullets();
    void updateAsteroidPos();

    /**
     * @brief   Adds a bullet to the scene. Takes ownership of the given pointer
     */
    void addBullet(Bullet::Ptr& bullet);

    void addEnemyBullet(Bullet::Ptr& bullet);

    /**
     * @brief   Renders all objects and particle effects
     */
    void render();

    /**
     * @brief   The engine's main loop
     */
    void process();

    /**
     * @brief Fügt das Spielerraumschiff hinzu welches explodieren soll.
     */ 
    void addSpaceCraft(SpaceCraft::Ptr spacecraft);

    /**
     * @brief Fügt das Gegenspielerraumschiff hinzu welches explodieren soll.
     */ 
    void addEnemyPlayer(SpaceCraft::Ptr enemyPlayer);

    /**
     * @brief removeBullets remove marked bullets
     * @return
     */
    int removeBullets();

    /**
     * @brief removeAster removes marked asateroids
     * @return
     */
    int removeAster();

    /**
     * @brief resets the engne state
     */
    void reset();

private:

    /// List of destroyable objects
    list<Asteroid::Ptr>    m_objects;

    /// List of active bullets
    list<Bullet::Ptr>            m_bullets;

    ///Raumschiff das explodieren soll
    SpaceCraft::Ptr              m_spacecraft;

    SpaceCraft::Ptr              m_enemyPlayer;

    ParticleEngine               m_particles;
};

} /* namespace asteroids */
#endif /* PHYSICSENGINE_HPP_ */
